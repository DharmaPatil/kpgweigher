Imports Microsoft.Office.Core
imports Extensibility
Imports System.Runtime.InteropServices
Imports Microsoft.Office.Core.MsoControlType
Imports System.Windows.Forms



#Region " Read me for Add-in installation and setup information. "
' When run, the Add-in wizard prepared the registry for the Add-in.
' At a later time, if the Add-in becomes unavailable for reasons such as:
'   1) You moved this project to a computer other than which is was originally created on.
'   2) You chose 'Yes' when presented with a message asking if you wish to remove the Add-in.
'   3) Registry corruption.
' you will need to re-register the Add-in by building the MyAddin1Setup project 
' by right clicking the project in the Solution Explorer, then choosing install.
#End Region

'<GuidAttribute("1DB934B1-0DAC-469C-B934-28935731314D"), ProgIdAttribute("RT96WordAddin.Connect")> _
<GuidAttribute("C28943AF-36C1-4f8a-84E7-47706A046779"), ProgIdAttribute("RT96WordAddin.Connect")> _
Public Class Connect
    Implements Extensibility.IDTExtensibility2

    Dim b_runing As Boolean
    Dim WithEvents DCStart As CommandBarButton         '�˵�����ť��ע�����WithEvents�ؼ�Ŷ���Ǵ����¼��ģ�
    Dim WithEvents DCEnd As CommandBarButton           '�˵�����ť��ע�����WithEvents�ؼ�Ŷ���Ǵ����¼��ģ�
    Dim WithEvents DCConfig As CommandBarButton        '�˵�����ť��ע�����WithEvents�ؼ�Ŷ���Ǵ����¼��ģ�
    Dim applicationObject As Object
    Dim addInInstance As Object
    Dim cfgdlg As ConfigDlg

    '����ʼ�رռ��غ�ʱ��ִ�и��¼�����
    Public Sub OnBeginShutdown(ByRef custom As System.Array) Implements Extensibility.IDTExtensibility2.OnBeginShutdown
        On Error Resume Next
        If DCStart Is Nothing Then
            Return
        End If
        DCStart.Delete()                                                     'ɾ�����غ꽨���Ĳ˵�����ť
        DCStart = Nothing                                                   '����˵�����ť�ڴ�ռ�
        DCEnd.Delete()                                                     'ɾ�����غ꽨���Ĳ˵�����ť
        DCEnd = Nothing                                                   '����˵�����ť�ڴ�ռ�
        DCConfig.Delete()                                                     'ɾ�����غ꽨���Ĳ˵�����ť
        DCConfig = Nothing                                                   '����˵�����ť�ڴ�ռ�
        cfgdlg = Nothing


    End Sub

    '����COM���غ�ж�ػ����ʱִ�����´���
    Public Sub OnAddInsUpdate(ByRef custom As System.Array) Implements Extensibility.IDTExtensibility2.OnAddInsUpdate
    End Sub

    '����������Ϻ�ִ�иù���
    Public Sub OnStartupComplete(ByRef custom As System.Array) Implements Extensibility.IDTExtensibility2.OnStartupComplete
        Dim oCommandBars As CommandBars
        Dim oStandardBar As CommandBar
        Dim oBarPopup As CommandBarPopup
        Dim oControl As CommandBarControl
        Dim i As Integer
        On Error Resume Next
        oCommandBars = applicationObject.CommandBars                                        '��ȡWordӦ�ó����CommandBars����
        oStandardBar = oCommandBars.Item("Standard")                                           '��ȡ����׼��CommandBar����

        oBarPopup = oStandardBar.Controls.Add(Type:=msoControlPopup, Temporary:=True)  '��ӵ����˵���
        oBarPopup.Caption = "RT9604AddIn"                                                                                    '���õ����˵���Caption����
        Me.b_runing = False


        DCStart = oBarPopup.Controls.Add(Type:=msoControlButton)                                       'Ϊ�˵�����Ӱ�ť
        With DCStart
            .Caption = "��ʼ�ɼ�����"                                                                               '���ò˵���ť��Caption����
            .Style = MsoButtonStyle.msoButtonCaption                                                                  'ָ����ť��style����
            .Visible = True            '���ð�ť�ɼ�
        End With
        DCEnd = oBarPopup.Controls.Add(Type:=msoControlButton)                                       'Ϊ�˵�����Ӱ�ť
        With DCEnd
            .Caption = "ֹͣ�ɼ�����"                                                                               '���ò˵���ť��Caption����
            .Style = MsoButtonStyle.msoButtonCaption                                                                  'ָ����ť��style����
            .Visible = True            '���ð�ť�ɼ�
        End With
        DCConfig = oBarPopup.Controls.Add(Type:=msoControlButton)                                       'Ϊ�˵�����Ӱ�ť
        With DCConfig
            .Caption = "�ɼ�ѡ������.."                                                                               '���ò˵���ť��Caption����
            .Style = MsoButtonStyle.msoButtonCaption                                                                  'ָ����ť��style����
            .Visible = True            '���ð�ť�ɼ�
        End With
        Me.UpdateButtonStatus()


        oStandardBar = Nothing
        oCommandBars = Nothing

        BuildCfgDlg()
        Me.cfgdlg.Visible = False
    End Sub
    Private Sub BuildCfgDlg()
        If Me.cfgdlg Is Nothing Then
            Me.cfgdlg = New ConfigDlg
            Me.cfgdlg.m_appObject = applicationObject                                                                          'ָ�������m_appObject����ֵ
            Me.cfgdlg.Visible = False
            'Me.cfgdlg.Show()

        End If
    End Sub

    Private Sub UpdateButtonStatus()
        DCStart.Enabled = Not Me.b_runing
        DCEnd.Enabled = Me.b_runing
        DCConfig.Enabled = Not Me.b_runing
    End Sub

    Public Sub OnDisconnection(ByVal RemoveMode As Extensibility.ext_DisconnectMode, ByRef custom As System.Array) Implements Extensibility.IDTExtensibility2.OnDisconnection
        On Error Resume Next
        If RemoveMode <> Extensibility.ext_DisconnectMode.ext_dm_HostShutdown Then
            OnBeginShutdown(custom)
        End If
        applicationObject = Nothing
    End Sub

    Public Sub OnConnection(ByVal application As Object, ByVal connectMode As Extensibility.ext_ConnectMode, ByVal addInInst As Object, ByRef custom As System.Array) Implements Extensibility.IDTExtensibility2.OnConnection

        applicationObject = application
        addInInstance = addInInst


        If (connectMode <> Extensibility.ext_ConnectMode.ext_cm_Startup) Then
            OnStartupComplete(custom)
        End If
    End Sub

    '�˵���ť�ĵ����¼�����
    Private Sub DCStart_Click(ByVal Ctrl As Microsoft.Office.Core.CommandBarButton, ByRef CancelDefault As Boolean) Handles DCStart.Click
        '    Dim fm As New frmHide                                                                                               'ʵ����frmHide����
        '   fm.m_appObject = applicationObject                                                                          'ָ�������m_appObject����ֵ
        '  fm.Show()                 
        '��ʾ����

        If Me.b_runing = False Then
            Me.cfgdlg.Visible = False
            Me.cfgdlg.StartRun()
            Me.b_runing = True
            Me.UpdateButtonStatus()

        End If
    End Sub
    Private Sub DCEnd_Click(ByVal Ctrl As Microsoft.Office.Core.CommandBarButton, ByRef CancelDefault As Boolean) Handles DCEnd.Click
        If Me.b_runing = True Then
            Me.cfgdlg.Visible = False
            Me.cfgdlg.StopRun()
            Me.b_runing = False
            Me.UpdateButtonStatus()
        End If

    End Sub
    Private Sub DCConfig_Click(ByVal Ctrl As Microsoft.Office.Core.CommandBarButton, ByRef CancelDefault As Boolean) Handles DCConfig.Click
        If Me.b_runing Then _
            Return

        Me.cfgdlg.Visible = True
        Me.cfgdlg.Show()
    End Sub

End Class


