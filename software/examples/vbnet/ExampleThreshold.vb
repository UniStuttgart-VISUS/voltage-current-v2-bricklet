Imports System
Imports Tinkerforge

Module ExampleThreshold
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Voltage/Current Bricklet 2.0

    ' Callback subroutine for power callback
    Sub PowerCB(ByVal sender As BrickletVoltageCurrentV2, ByVal power As Integer)
        Console.WriteLine("power: " + (power/1000.0).ToString() + " W")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim vc As New BrickletVoltageCurrentV2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register power callback to subroutine PowerCB
        AddHandler vc.PowerCallback, AddressOf PowerCB

        ' Configure threshold for power "greater than 10 W"
        ' with a debounce period of 1s (1000ms)
        vc.SetPowerCallbackConfiguration(1000, False, ">"C, 10*1000, 0)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
