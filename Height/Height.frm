VERSION 5.00
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.1#0"; "MSCOMM32.OCX"
Begin VB.Form TempForm 
   Caption         =   "HEIGHT"
   ClientHeight    =   8250
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   13170
   LinkTopic       =   "Form1"
   ScaleHeight     =   8250
   ScaleMode       =   0  'User
   ScaleWidth      =   13174.7
   StartUpPosition =   3  '����ȱʡ
   Begin VB.CommandButton Command1 
      Caption         =   "����"
      Height          =   375
      Left            =   9240
      TabIndex        =   34
      Top             =   240
      Width           =   1215
   End
   Begin MSCommLib.MSComm MSComm_Temp 
      Left            =   11400
      Top             =   120
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   393216
      CommPort        =   4
      DTREnable       =   -1  'True
      BaudRate        =   115200
   End
   Begin VB.ComboBox Combo_Com 
      Height          =   300
      Left            =   3480
      TabIndex        =   2
      Text            =   "COM1"
      Top             =   240
      Width           =   2199
   End
   Begin VB.Timer Timer1 
      Interval        =   1
      Left            =   12240
      Top             =   240
   End
   Begin VB.CommandButton Comm_open 
      Caption         =   "�򿪴���"
      Height          =   375
      Left            =   7200
      TabIndex        =   1
      Top             =   240
      Width           =   1350
   End
   Begin VB.PictureBox Templine 
      BackColor       =   &H00FFFFFF&
      Height          =   6735
      Left            =   1200
      ScaleHeight     =   8000
      ScaleMode       =   0  'User
      ScaleWidth      =   22700
      TabIndex        =   0
      TabStop         =   0   'False
      Top             =   800
      Width           =   11400
   End
   Begin VB.Label Label32 
      Caption         =   "300"
      Height          =   150
      Left            =   840
      TabIndex        =   33
      Top             =   840
      Width           =   435
   End
   Begin VB.Label Label31 
      Caption         =   "250"
      Height          =   150
      Left            =   840
      TabIndex        =   32
      Top             =   1560
      Width           =   435
   End
   Begin VB.Label Label30 
      Caption         =   "90"
      Height          =   195
      Left            =   9996
      TabIndex        =   31
      Top             =   7680
      Width           =   300
   End
   Begin VB.Label Label29 
      Caption         =   "85"
      Height          =   195
      Left            =   9497
      TabIndex        =   30
      Top             =   7680
      Width           =   300
   End
   Begin VB.Label Label28 
      Caption         =   "80"
      Height          =   195
      Left            =   8997
      TabIndex        =   29
      Top             =   7680
      Width           =   300
   End
   Begin VB.Label Label27 
      Caption         =   "75"
      Height          =   195
      Left            =   8497
      TabIndex        =   28
      Top             =   7680
      Width           =   300
   End
   Begin VB.Label Label26 
      Caption         =   "70"
      Height          =   195
      Left            =   7997
      TabIndex        =   27
      Top             =   7680
      Width           =   300
   End
   Begin VB.Label Label25 
      Caption         =   "65"
      Height          =   195
      Left            =   7497
      TabIndex        =   26
      Top             =   7680
      Width           =   300
   End
   Begin VB.Label Label24 
      Caption         =   "60"
      Height          =   195
      Left            =   6997
      TabIndex        =   25
      Top             =   7680
      Width           =   300
   End
   Begin VB.Label Label23 
      Caption         =   "55"
      Height          =   195
      Left            =   6498
      TabIndex        =   24
      Top             =   7680
      Width           =   300
   End
   Begin VB.Label Label22 
      Caption         =   "50"
      Height          =   195
      Left            =   5998
      TabIndex        =   23
      Top             =   7680
      Width           =   300
   End
   Begin VB.Label Label21 
      Caption         =   "45"
      Height          =   195
      Left            =   5498
      TabIndex        =   22
      Top             =   7680
      Width           =   195
   End
   Begin VB.Label Label20 
      Caption         =   "40"
      Height          =   195
      Left            =   4998
      TabIndex        =   21
      Top             =   7680
      Width           =   195
   End
   Begin VB.Label Label19 
      Caption         =   "35"
      Height          =   195
      Left            =   4498
      TabIndex        =   20
      Top             =   7680
      Width           =   195
   End
   Begin VB.Label Label18 
      Caption         =   "30"
      Height          =   195
      Left            =   3999
      TabIndex        =   19
      Top             =   7680
      Width           =   195
   End
   Begin VB.Label Label17 
      Caption         =   "25"
      Height          =   195
      Left            =   3499
      TabIndex        =   18
      Top             =   7680
      Width           =   195
   End
   Begin VB.Label Label16 
      Caption         =   "20"
      Height          =   195
      Left            =   2999
      TabIndex        =   17
      Top             =   7680
      Width           =   195
   End
   Begin VB.Label Label15 
      Caption         =   "15"
      Height          =   195
      Left            =   2499
      TabIndex        =   16
      Top             =   7680
      Width           =   195
   End
   Begin VB.Label Label14 
      Caption         =   "10"
      Height          =   195
      Left            =   1999
      TabIndex        =   15
      Top             =   7680
      Width           =   195
   End
   Begin VB.Label Label13 
      Caption         =   "5"
      Height          =   195
      Left            =   1500
      TabIndex        =   14
      Top             =   7680
      Width           =   195
   End
   Begin VB.Label Label12 
      Caption         =   "200"
      Height          =   150
      Left            =   840
      TabIndex        =   13
      Top             =   2400
      Width           =   435
   End
   Begin VB.Label Label11 
      Caption         =   "0"
      Height          =   255
      Left            =   1000
      TabIndex        =   12
      Top             =   7680
      Width           =   135
   End
   Begin VB.Label Label10 
      Caption         =   "-50"
      Height          =   150
      Left            =   840
      TabIndex        =   11
      Top             =   6600
      Width           =   435
   End
   Begin VB.Label Label9 
      Caption         =   "-100"
      Height          =   150
      Left            =   720
      TabIndex        =   10
      Top             =   7440
      Width           =   375
   End
   Begin VB.Label Label8 
      Caption         =   "0"
      Height          =   150
      Left            =   960
      TabIndex        =   9
      Top             =   5760
      Width           =   555
   End
   Begin VB.Label Label7 
      Caption         =   "50"
      Height          =   150
      Left            =   960
      TabIndex        =   8
      Top             =   4920
      Width           =   435
   End
   Begin VB.Label Label6 
      Caption         =   "100"
      Height          =   150
      Left            =   840
      TabIndex        =   7
      Top             =   4080
      Width           =   315
   End
   Begin VB.Label Label5 
      Caption         =   "ʱ��(S)"
      Height          =   255
      Left            =   10296
      TabIndex        =   6
      Top             =   7680
      Width           =   800
   End
   Begin VB.Label Label4 
      Caption         =   "150"
      Height          =   150
      Left            =   840
      TabIndex        =   5
      Top             =   3240
      Width           =   435
   End
   Begin VB.Label Label3 
      Caption         =   "Hright��mm��"
      Height          =   495
      Left            =   600
      TabIndex        =   4
      Top             =   360
      Width           =   1215
   End
   Begin VB.Label Label2 
      Caption         =   "���ں�"
      BeginProperty Font 
         Name            =   "����"
         Size            =   12
         Charset         =   134
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   2280
      TabIndex        =   3
      Top             =   240
      Width           =   855
   End
End
Attribute VB_Name = "TempForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim flag_com_open As Boolean                '����COM�˿ڵĴ򿪱�ʶ
Dim time_temp As Integer                    '�����¼ʱ��ı���
Dim temp_bufx, temp_bufy As Integer         '���������������ı���

Private Sub Combo_Com_Click()
    On Error Resume Next
    x = Combo_Com.Text                      '��ȡѡ����б�����ı�
    m = Len(x)                              '��ȡ�б���ĳ���
    n = Val(Right$(x, m - 3))               '��ȡ�б�����Ķ˿ں�
    MSComm_Temp.CommPort = n                '����MScomm�ؼ��Ķ˿ں�Ϊѡ��Ķ˿ں�
End Sub

Private Sub Comm_open_Click()
    If (flag_com_open = False) Then
       Comm_open.Caption = "�رն˿�"       '����flag_com_open��ֵ���ð�ťComm_open��Caption��ֵ
       flag_com_open = True
       If MSComm_Temp.PortOpen = False Then
           MSComm_Temp.PortOpen = True
       End If
       Timer1.Enabled = True                '�򿪶˿ں�ʹTimer1��Ч����ʼ��ʱ
    Else
       Comm_open.Caption = "�򿪶˿�"
       MSComm_Temp.PortOpen = False
       flag_com_open = False
       Timer1.Enabled = False               '�رն˿ں�ʹTimer1��Ч
    End If
End Sub

Private Sub Command1_Click()
    Templine.Cls
    Templine.DrawStyle = 2                      '�����ߵĸ�ʽ��Ϊ���߸�ʽ
    For i = 0 To 8000 Step 500                '���λ����¶ȵĸ�����׼��
        Templine.Line (0, i)-(30000, i)
    Next i
    Templine.DrawStyle = 1
    time_temp = 0
End Sub

Private Sub Form_Load()
    Dim i As Integer
    Timer1.Enabled = False                      '���ö�ʱ��Timer1��Ч
    Timer1.Interval = 1000                      '�趨��ʱ���Ĵ���ʱ����1000ms
    temp_bufx = 0
    temp_bufy = 5500
    Templine.AutoRedraw = True
    Templine.DrawStyle = 2                      '�����ߵĸ�ʽ��Ϊ���߸�ʽ
    For i = 0 To 8000 Step 500                '���λ����¶ȵĸ�����׼��
        Templine.Line (0, i)-(9250, i)
    Next i
    Templine.DrawStyle = 0                      '�����ߵĸ�ʽ��Ϊʵ�߸�ʽ
    For i = 1 To 16                             '����Ϊ�����б������б���
        Combo_Com.AddItem "COM" & i
    Next i
    MSComm_Temp.Settings = "9600,N,8,1"         '��������9600����У�飬8Ϊ����λ��1λֹͣλ
    MSComm_Temp.OutBufferSize = 0               '���÷��ͻ������Ĵ�С�����ַ�Ϊ��λ
    MSComm_Temp.InBufferSize = 0                '���ý��ջ������Ĵ�С�����ַ�Ϊ��λ
    MSComm_Temp.InputMode = comInputModeText    '���ı���ʽ����
    MSComm_Temp.RThreshold = 4                  '���ò���oncomm�¼����ַ��������ַ�Ϊ��λ
    MSComm_Temp.SThreshold = 1
    MSComm_Temp.InputLen = 0                    '���ôӽ��ջ�������ȡ���ַ�����Ϊ0��ʾȫ����ȡ
    MSComm_Temp.InBufferCount = 0               '��ս��ջ�����
End Sub

Private Sub MSComm_Temp_OnComm()
    Dim a As String
    Dim temp As Integer
    If MSComm_Temp.CommEvent = comEvReceive Then
        a = MSComm_Temp.Input                   '��ȡ�������ڵ�����
        For i = 1 To 3
            temp = temp * 10 + Val(Mid(a, i, 1)) '�������ݣ��õ����������ֵ
        Next i
        'Print (temp)
        If time_temp <> 0 Then
             Templine.Line (temp_bufx, temp_bufy)-(time_temp * 30, 10000 - 20 * temp), vbRed '���ݵõ�������ֵ�͵�ǰ��ʱ���Լ�ԭ�е����ݻ���һ��ֱ��
        End If
        temp_bufx = time_temp * 30              '�洢��ǰ���ݣ��Է����´�ʹ��
        temp_bufy = 10000 - 20 * temp
        time_temp = time_temp + 1               'ʱ��ֵ����1
    End If
End Sub

Private Sub Timer1_Timer()                      'Timer1�жϴ�����
    MSComm_Temp.Output = Chr(&H30)              '����λ������һ�����ݣ����ں���λ��ͨ��
End Sub
