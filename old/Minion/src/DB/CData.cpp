#include <cassert>

#include "DB/CData.h"

#include "StructDefines/structdefines.h"

#include "Config/CPreferences.h"
#include "DB/CDatabase.h"
#include "Regression/CRegressionObject.h"

#include "Util/Util.h"
#include "Util/Util_logging.h"



std::string PrintPtStr(double* point, int ndims)			
{
	// print point
	std::string out_str = string("(") + ToString(point[0]);
	for(int i = 1; i < ndims; i++) 
		out_str += (", " + ToString(point[i]));
	out_str += string(")");

	return out_str;
}


CDataObject::CDataObject(int total_pts, enumStreetIndices br_ndx, int times_acted, int ndealt, eBetType action_type) : 
	m_npoints(total_pts), 
	m_br(br_ndx), 
	m_nacted(times_acted),
	m_ndealt(ndealt),
	m_action(action_type)
{
	assert(br_ndx >= ePreflopIndex && br_ndx < eRoundIndices);
	assert(times_acted >= 0);

	m_ndims = times_acted + (br_ndx == ePreflopIndex ? num_prior_dims_preflop : num_prior_dims_postflop) + 1;

	// this is only used for ann
	m_data = new double*[m_npoints];
	// allocate hand_ids for corresponding data points
	m_hand_ids = new long[m_npoints];
	for(long i = 0; i < m_npoints; i++)
		m_hand_ids[i] = -1L;

	m_profits	= VectorXd::Zero(m_npoints);
	m_points	= Matrix<double, Dynamic, Dynamic, RowMajor>::Zero(m_npoints, m_ndims);

	CDatabase p_db;
	GetData(&p_db);

	// Set the weights
	diag_factor	= Matrix<double, Dynamic, Dynamic, RowMajor>::Identity(m_ndims, m_ndims);

	#ifdef KASPER_WEIGHTS

		RowVectorXd tmp;
		if(ePreflopIndex == br_ndx)
		{
			tmp = RowVectorXd::Zero(9);
			tmp << 10, 20, 100, 10, 0, 0, 2, 2, 20;
		}
		else
		{
			tmp = RowVectorXd::Zero(11);
			tmp << 10, 80, 10, 0, 0, 0, 2, 2, 50, 1, 1;
		}
		diag_factor.bottomRightCorner(tmp.cols(), tmp.cols()) = tmp.asDiagonal();

	#else

		FeatureNormalize();
		CRegressionObject regress(m_points, m_profits);
		diag_factor = regress.get_theta().asDiagonal();

	#endif

	gLog.WriteLog(eSeverityInfo, eCatPerformance, "br%d_%d: %8s - %8d points\n", br_ndx+1, times_acted, bets_str[action_type], m_npoints);
}

CDataObject::~CDataObject()
{
	CleanUp();
}

void CDataObject::CleanUp()
{
	delete [] m_hand_ids;
	delete [] m_data;
}

void CDataObject::FeatureNormalize()
{
	dim_means = VectorXd::Zero(m_ndims);
	dim_std = VectorXd::Ones(m_ndims);

	for (int i = 1; i < m_points.cols(); i++)
	{
		double mean = m_points.col(i).mean();
		// Save the mean for dealing with new points
		dim_means(i) = mean;
		// 0 center data
		VectorXd meansColVector = VectorXd::Constant(m_points.rows(), mean);
		m_points.col(i) -= meansColVector;
		
		#define NORMALIZE_DIST
		#ifdef NORMALIZE_DIST
			// divide by std
			double sig_squared = (m_points.col(i).array() - m_points.col(i).mean()).square().sum() / 
											(m_points.rows() > 1 ? m_points.rows()-1 : 1);
			if(sig_squared > 0)
			{
				m_points.col(i).array() *= 1.0/std::sqrt(sig_squared);
				dim_std(i) =  1.0/std::sqrt(sig_squared);
			}
		#endif
	}
}

long CDataObject::GetData(CDatabase* p_db)
{
	/*
		Fills the data array and returns the number of points inserted
	*/

	assert(m_br >= ePreflopIndex && m_br < eRoundIndices);
	assert(m_nacted >= 0);

	std::string query_str = CDatabaseInterface::ConstructSelectQueryString(m_br, m_nacted, m_ndealt, m_action);

	//p_db->CheckDB(br_ndx, times_acted);
	CDatabaseQuery db_query(p_db);
	db_query.Select(query_str.c_str());

	// Get the actual values
	for(int p = 0; p < db_query.get_numrows(); p++)
	{
		// save address for ann
		m_data[p] = m_points.row(p).data();
		// save hand_id
		m_hand_ids[p] = atoi(PQgetvalue(db_query.get_pgresult(), p, 0));
		// FIX ME - profit should be a part of the select query
		bool profit_ok = CDatabaseInterface::GetProfit(m_hand_ids[p], m_profits(p), p_db);

		// prefix every row with 1's for teh intercept
		m_points(p, 0) = 1.0;
		for(int column = 1; column < db_query.get_numcolumns(); column++)
		{
			double tmp = atof(PQgetvalue(db_query.get_pgresult(), p, column));
			m_points(p, column) = tmp;
		}

		#if defined DEBUG_ALIGN
			if(row < 10)
			{
				double progress_percent = ((m_npoints > 0) ? ((100.0 * (row+1))/m_npoints) : 0.0);
				gLog.WriteLogNoHeading(eSeverityInfo, eCatANN, "Data Point : %d/%d [%.2lf %%] - %s\n", 
														row+1, m_npoints, progress_percent, PrintPtStr(m_points.row(row).data(), m_ndims).c_str());
			}
		#endif
	}

	return db_query.get_numrows();
}

