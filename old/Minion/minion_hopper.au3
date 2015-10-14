; Poker Academy Short Stack, Stack Resetter
; by quantpoker
; based off of Poker Academy SNG Hopper by SnG.Champ
; 2011 - Merged with Aiku's title stuff by strikles


;INCLUDES

#include <File.au3>
#include <GuiConstants.au3>
#include <SendMessage.au3>


;VARIABLES
 ; Set $szIPADDRESS to wherever the SERVER is. We will change a PC name into an IP Address
 ;   Local $szServerPC = @ComputerName
 ;   Local $szIPADDRESS = TCPNameToIP($szServerPC)
Dim $szIPADDRESS = "192.168.56.101"
Dim $nPORT = 7777
; Initialize a variable to represent a connection
;==============================================
Dim $ConnectedSocket = -1

Dim $bb = "1", $sb = "0.5", $type = "NL"
Dim $casino_name = "Poker Academy Pro"
Dim $casino_hwnd = ""
Dim $OH_hwnds_list, $Client_hwnds_list
Dim $playing = 1, $ClientExists = 1
Dim $varDealHandColor

MainLogic()

Func MainLogic()
	InitialSetup()
	While $playing == 1
		If (WinExists($casino_name)) Then
			If ($ClientExists == 0) Then
				InitialSetup()
			EndIf
			MaintainWindowConditions()
			Rebuy()
			DealHand()
			Global $ClientExists = 1
		Else
			Global $ClientExists = 0
		EndIf
		Sleep (625)
   WEnd
EndFunc

Func InitialSetup()
	If (WinExists($casino_name)) Then
		GUICreate("Poker Academy title set", 180, 240, 0, 0)

		GUICtrlCreateLabel("Small Blind", 10, 10, 80, 20)
		Local $s = GUICtrlCreateInput("0.50", 90, 10, 40, 20)

		GUICtrlCreateLabel("Big Blind", 10, 40, 80, 20)
		Local $b = GUICtrlCreateInput("1", 90, 40, 40, 20)

		GUICtrlCreateLabel("Game type", 10, 70, 80, 20)
		Local $ty = GUICtrlCreateCombo("NL", 90, 70, 80, 20)
		GUICtrlSetData(-1, "NL|PL|FL")
		Local $start = GUICtrlCreateButton("Start", 40, 150, 100, 40)

		GUISetState()
		Do
			Local $msg = GUIGetMsg()
		Until $msg = $GUI_EVENT_CLOSE Or $msg = $start
		GUISetState(@SW_HIDE)

		Global $type = GUICtrlRead($ty)
		Global $bb = GUICtrlRead($b)
		Global $sb = GUICtrlRead($s)

		WinSetTitle($casino_name, "", $casino_name & " - " & $sb & "/" & $bb & " " & $type)
	EndIf
EndFunc

Func SetActive()
	; Activates window and moves it to the origin
	If WinActive ($casino_name) == 0 Then
	;   dbg ( "Window is not active. Reactivating..." )
		WinActivate ($casino_name)
	EndIf
EndFunc

Func MaintainPosition()
	Local $winpos
	$winpos = WinGetPos($casino_name)
	If (($winpos[0] > 0) OR ($winpos[1] > 0)) Then
		;dbg ( "Window is not at origin. Moving back to 0.0..." )
		WinMove ( "Poker Academy Pro", "", 0, 0) ; Moves window to 0,0 coordinates
	EndIf
	WinActivate ($casino_name)
EndFunc

Func MaintainWindowConditions()
	SetActive()
	MaintainPosition()
	WinSetTitle($casino_name, "", $casino_name & " - " & $sb & "/" & $bb & " " & $type)
EndFunc

Func Rebuy()
	
	;case 1
	If (PixelGetColor(630, 315, $casino_name) == (0x4B1515)) Then ; Not enough Chips
		resetStacks()
	EndIf
	
	; case 2
	If (PixelGetColor(387, 535, $casino_name) == (0x5D1B1B) And PixelGetColor(410, 542, $casino_name) == (0x5D1B1B)) Then ; Add Chips
		resetStacks()
	EndIf
	
	; deal with Abort hand
	If (PixelGetColor(500, 310, $casino_name) == (0xD4D0C8)) Then
		Sleep(5)
		MouseClick ( "", 353, 313) ; Abort Current Hand
		Sleep(5)
	EndIf
	
	; Set all bankrolls
	; Will use this for socket message
	If (PixelGetColor(550, 203, $casino_name) == (0xFFFFFF)) Then
		Sleep(5)
		MouseClick ( "",300, 200) ; Set all Bankrolls
		Sleep(5)
		MouseClick ( "",345, 435) ; OK Button
		Sleep(5)
		SendRebuyMessage()
	EndIf
EndFunc

Func DealHand()
	If(PixelGetColor(352, 535, $casino_name) == (0x5D1B1B) And PixelGetColor(423, 535, $casino_name) == (0x5D1B1B)) Then
		Sleep (5)
		MouseClick ( "", 400, 540) ; Deal Hand button
		Sleep(5)
	EndIf
 EndFunc
 
 
Func ResetStacks()
	Sleep(5)
	MouseClick ( "",165,30) ; Table
	Sleep(5)
	MouseClick ( "",165,150) ; Set All Bankrolls
	Sleep(5)
EndFunc
 
Func UpdateWindowLists()
   Global $OH_hwnds_list = WinList("[CLASS:OpenHoldem]", "")
   Global $Client_hwnds_list = WinList($casino_Name)
   If @error Then
	  MsgBox(4096, "Hopper Error", "UpdateWindowList()" & @LF & "Could not find the correct window")
   Else
	  ; just 1 window for testing now
	  Global $casino_hwnd = Hex($Client_hwnds_list[1][1])
   EndIf
EndFunc

Func SendRebuyMessage()
	UpdateWindowLists()
	Local $packet_sent = 0
	For $i = 1 To $OH_hwnds_list[0][0]
		Global $connected_hwnd = Hex(_SendMessage($OH_hwnds_list[$i][1], 0x8000+4, 0, 0))
		If $connected_hwnd == $casino_hwnd Then
			; SendMessage($WindowHandle, Message, wParam=0, lParam=flag_number)
			;_SendMessage($OHhwnd[$i][1], 0x8000+5, 0, 1) ; Enable Flag1
			SendProfitPacket($connected_hwnd)
			$packet = 1
			Return
		EndIf
	Next
	If $packet_sent == 0 Then
		MsgBox(64, "Dafuq: ", " Profit Packet was not sent :" & @LF & $connected_hwnd & @LF & $casino_hwnd)
	EndIf
EndFunc

Func InitializeTCP()
	; Start The TCP Services
	TCPStartup()
	;Attempt to connect to SERVER at its IP and PORT 
	Global $ConnectedSocket = TCPConnect($szIPADDRESS, $nPORT)
	; If there is an error... show it
	If @error Then
		MsgBox(4112, "Hopper Error", "InitializeTCP TCPConnect failed with WSA error: " & @error)
		Return -1
	Else
		Return $ConnectedSocket
	EndIf
EndFunc

; Assuming Hopper + OH on the same machine 
; and only 1 instance of each per machine this will work
Func SendProfitPacket($connected_hwnd_str)
	Local $szData = "10240009" & $connected_hwnd_str
	Global $ConnectedSocket = InitializeTCP()
	
	TCPSend($ConnectedSocket, StringToBinary($szData, 1))
	If @error Then
		MsgBox(4112, "Hopper Error", "SendProfitPacket TCPSend failed with WSA error: " & @error)
		Global $ConnectedSocket = InitializeTCP()
		TCPSend($ConnectedSocket, StringToBinary($szData, 1))
		If @error Then
			MsgBox(4112, "SendProfitPacket TCPSend failed with WSA error: " & @error)
		EndIf
	EndIf
	
	TCPCloseSocket($ConnectedSocket)
	TCPShutdown()
EndFunc   
