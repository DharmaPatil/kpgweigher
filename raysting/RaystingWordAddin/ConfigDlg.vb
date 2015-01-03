Imports System.Random
Imports System.Windows.Forms
Imports System.Runtime.InteropServices
Imports System.Threading
Imports System.Globalization
Imports System.Text.RegularExpressions
Imports Microsoft.Office.Interop



Public Class ConfigDlg
    Inherits System.Windows.Forms.Form
    Dim b_run As Boolean        'running or not
    Dim b_formula As Boolean    'match the formula or not
    Dim i_lastfield As Integer  'field index of last selection
    Dim s_port As String
    Dim s_found As String       'string to find
    Dim once As Boolean
    Dim rand As Random


    Dim moRS232 As Rs232


#Region " Windows ������������ɵĴ��� "

    Public Sub New()
        MyBase.New()

        '�õ����� Windows ���������������ġ�
        InitializeComponent()

        '�� InitializeComponent() ����֮������κγ�ʼ��
        once = False
        rand = New Random(123)
        b_run = False
        s_port = ""
        i_lastfield = -1
        moRS232 = New Rs232
        Me.CBPort.SelectedIndex = 0
    End Sub

    '������д dispose ����������б�
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then
            If Not (components Is Nothing) Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub

    'Windows ����������������
    Private components As System.ComponentModel.IContainer

    'ע��: ���¹����� Windows ����������������
    '����ʹ�� Windows ����������޸Ĵ˹��̡�
    '��Ҫʹ�ô���༭���޸�����
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents Timer1 As System.Windows.Forms.Timer
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents CBPort As System.Windows.Forms.ComboBox
    Friend WithEvents BtnOk As System.Windows.Forms.Button
    Friend WithEvents BtCancel As System.Windows.Forms.Button
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container
        Me.Label1 = New System.Windows.Forms.Label
        Me.CBPort = New System.Windows.Forms.ComboBox
        Me.Timer1 = New System.Windows.Forms.Timer(Me.components)
        Me.Label3 = New System.Windows.Forms.Label
        Me.BtnOk = New System.Windows.Forms.Button
        Me.BtCancel = New System.Windows.Forms.Button
        Me.SuspendLayout()
        '
        'Label1
        '
        Me.Label1.Location = New System.Drawing.Point(32, 24)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(64, 16)
        Me.Label1.TabIndex = 1
        Me.Label1.Text = "���ں�"
        '
        'CBPort
        '
        Me.CBPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.CBPort.Items.AddRange(New Object() {"COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9", "COM10"})
        Me.CBPort.Location = New System.Drawing.Point(128, 16)
        Me.CBPort.Name = "CBPort"
        Me.CBPort.Size = New System.Drawing.Size(112, 20)
        Me.CBPort.TabIndex = 2
        '
        'Timer1
        '
        Me.Timer1.Enabled = True
        Me.Timer1.Interval = 1000
        '
        'Label3
        '
        Me.Label3.Location = New System.Drawing.Point(32, 56)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(208, 24)
        Me.Label3.TabIndex = 5
        Me.Label3.Text = "������:9600, ��У�飬1λֹͣλ"
        '
        'BtnOk
        '
        Me.BtnOk.Location = New System.Drawing.Point(56, 88)
        Me.BtnOk.Name = "BtnOk"
        Me.BtnOk.Size = New System.Drawing.Size(88, 24)
        Me.BtnOk.TabIndex = 6
        Me.BtnOk.Text = "ȷ��"
        '
        'BtCancel
        '
        Me.BtCancel.Location = New System.Drawing.Point(192, 88)
        Me.BtCancel.Name = "BtCancel"
        Me.BtCancel.Size = New System.Drawing.Size(88, 24)
        Me.BtCancel.TabIndex = 6
        Me.BtCancel.Text = "ȡ��"
        '
        'ConfigDlg
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(6, 14)
        Me.ClientSize = New System.Drawing.Size(304, 125)
        Me.ControlBox = False
        Me.Controls.Add(Me.BtnOk)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.CBPort)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.BtCancel)
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "ConfigDlg"
        Me.ShowInTaskbar = False
        Me.Text = "Raysting Data Collector"
        Me.ResumeLayout(False)

    End Sub

#End Region

    Public WithEvents m_appObject As Word.Application                          '��Ӧ�ó����¼���WordӦ�ó������
    Private Sub Timer1_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer1.Tick
        If b_run = False Then _
            Return
        Try
            Dim doc As Word.Document
            Dim str As String
            Dim code As String
            Dim fld As Word.Field
            Dim cell As Word.Range
            If m_appObject.Selection.Fields.Count <> 1 Then _
                Return
            fld = m_appObject.Selection.Fields.Item(1)
            code = fld.Code.Text
            If i_lastfield >= 0 And i_lastfield = fld.Index Then
                If Me.moRS232.IsOpen Then
                    Try
                        moRS232.Write(":fetch?" & Chr(10))
                        Dim datastr As String
                        datastr = ""
                        While (1 = moRS232.Read(1))
                            datastr = datastr + moRS232.InputStreamString
                            If datastr.IndexOfAny(Chr(13) & Chr(10)) > 0 Then
                                Dim rv As Double
                                '    rv = rand.NextDouble()
                                rv = Double.Parse(datastr.Trim())
                                If Me.b_formula Then
                                    If code.ToString.IndexOf(Me.s_found) = 0 Then
                                        If (rv >= 0) Then
                                            code = code.Replace(Me.s_found, " =0+" + rv.ToString)
                                        Else
                                            code = code.Replace(Me.s_found, " =0" + rv.ToString)
                                        End If
                                        fld.Code.Text = code
                                        fld.Update()
                                        RefreshFields()
                                        i_lastfield = -1
                                    End If
                                End If
                                moRS232.ClearInputBuffer()
                                Me.Text = code
                                Exit While
                            End If
                        End While
                    Catch ex As Exception
                        Return
                    End Try
                End If
            Else
                StartRun()
            End If
        Catch ex As Exception
            Return
        End Try
    End Sub
    Public Sub RefreshFields()
        Try
            Dim fld As Word.Field
            Dim i As Integer
            i = m_appObject.ActiveDocument.Fields.Count
            While (i > 0)
                fld = m_appObject.ActiveDocument.Fields.Item(i)
                fld.Update()
                i = i - 1
            End While
        Catch ex As Exception
        End Try
    End Sub
    Public Sub StartRun()
        b_run = False
        Try
            If moRS232.IsOpen Then
                moRS232.Close()
            End If
            With moRS232
                .Port = Me.CBPort.SelectedIndex + 1                                                    '// Uses COM1 
                .BaudRate = 9600                                        ' // 2400 baud rate 
                .DataBit = 8                                                 '// 8 data bits                                                   
                .StopBit = Rs232.DataStopBit.StopBit_1         '// 1 Stop bit 
                .Parity = Rs232.DataParity.Parity_None           '// No Parity 
                .Timeout = 500                                             '// 500 ms of timeout admitted to get all required bytes 
            End With
            '// Initializes and Open 
            moRS232.Open()
            If moRS232.IsOpen Then
                'moRS232.Write("*RST\n*CLS\n:INIT:CONT ON;:ABORT\n:SENS:FUNC 'VOLT:DC'\n:SENSE:CHAN 1\n:sens:volt:rang:auto on\n:sens:volt:chan1:lpas off\n:SENS:VOLT:DC:NPLC 1\nVOLT:DIG 8\n:syst:azer on\n")
            Else
                MessageBox.Show("�����޷���")
            End If
        Catch ex As Exception
            MessageBox.Show("����ʧ��:" + ex.Message)
            Return
        End Try

        Dim doc As Word.Document
        Try
            Dim fld As Word.Field
            Dim d As Double
            Dim code As String
            Me.b_run = True
            Me.b_formula = False
            i_lastfield = -1

            If 0 = m_appObject.Selection.Fields.Count Then
                m_appObject.Selection.SelectCell()  '.GoToNext(Word.WdGoToItem.wdGoToField)
            End If

            If 1 = m_appObject.Selection.Fields.Count Then
                fld = m_appObject.Selection.Fields.Item(1)
                code = fld.Code.Text
                If (("" = code) Or (Not code.StartsWith(" =0"))) Then
                    i_lastfield = fld.Index
                    Me.b_formula = False
                    Me.Timer1_Tick(Me, System.EventArgs.Empty)
                Else
                    Dim reg As New System.Text.RegularExpressions.Regex("^ =0(?<sign>[-\+]{1})(?<data>[-0-9.Ee\+]+)")
                    Dim m As System.Text.RegularExpressions.Match
                    m = reg.Match(code)
                    If m.Success Then
                        Me.s_found = " =0" + m.Groups("sign").Value.ToString + m.Groups("data").Value.ToString
                        i_lastfield = fld.Index
                        Me.b_formula = True
                        Me.Timer1_Tick(Me, System.EventArgs.Empty)
                    End If
                End If
            End If
        Catch ex As Exception
            Return
            'MessageBox.Show(ex.Message())
        End Try
    End Sub
    Public Sub StopRun()
        Me.b_run = False
        moRS232.Close()
    End Sub

    Private Sub BtCancel_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles BtCancel.Click
        If Me.s_port <> "" Then _
            Me.CBPort.SelectedText = Me.s_port
        Me.Visible = False
    End Sub

    Private Sub BtnOk_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles BtnOk.Click
        Me.s_port = Me.CBPort.SelectedText
        Me.Visible = False
    End Sub
End Class
