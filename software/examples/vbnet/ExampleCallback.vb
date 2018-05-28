Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Voltage/Current Bricklet 2.0

    ' Callback subroutine for current callback
    Sub CurrentCB(ByVal sender As BrickletVoltageCurrentV2, ByVal current As Integer)
        Console.WriteLine("Current: " + (current/1000.0).ToString() + " A")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim vc As New BrickletVoltageCurrentV2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register current callback to subroutine CurrentCB
        AddHandler vc.CurrentCallback, AddressOf CurrentCB

        ' Set period for current callback to 1s (1000ms) without a threshold
        vc.SetCurrentCallbackConfiguration(1000, False, "x"C, 0, 0)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
