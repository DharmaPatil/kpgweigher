﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Finisar.SQLite;
using System.ComponentModel;
using System.Data;
using System.Xml.Linq;
using System.IO;
using Raysting.Common;
namespace Zddq2
{
    /*
     * define channel info
     * Store the temperary variables during Rx measuring
     */
    public class RX_VAR
    {
        public double vRs;  //voltage on Rs
        public double vRx;  //voltage on Rs
        public double vCrossP; //cross voltage P
        public double vCrossN; //cross voltage N
        public double iSrc;    //current from current source , equals to (current go through Rx * iK ?);
        public double rRx;     //measured rRx
        public double rRs;     //Rs setting
        
        public int iK;         //current K value
        public string errMsg;   //store error message
        public RxInfo rx;
        public RX_VAR(RxInfo rxinfo)
        {
            rx = rxinfo;
            Reset();
        }
        public void Reset()
        {
            i_MeasCount = 0;
            i_State = RUN_STATE.IDLE;
            bRunning = false;
        }
        public static int INIT_LOOP = 800; //default K value on bootup
        public static int MAX_LOOP = 8191; //max loop available.
        //Get a new K value in non tracking mode
        //return the difference between new K and current k value
        public int calc_capture_nextk()
        {
            int N;
            N = Convert.ToInt32(Math.Floor(0.5+ rRx * INIT_LOOP / rRs));
            if (N < 1)    N = 1;
            if (N > MAX_LOOP) N = MAX_LOOP;
            return N - iK;
        }
        //Get a new K value in tracking mode                     
        //else use the weight algorithm
        public int calc_track_nextk()
        {
            int N;
            int sum;
            double dK = (double)iK / (double)INIT_LOOP;
            rRx = (dK * (rRs + ((vCrossP - vCrossN) / (2.0 * Math.Abs(iSrc)))));
            return calc_capture_nextk();
        }
        public int i_MeasCount //count of measure times
        {
            get;
            set;
        }
        public RUN_STATE i_State     //state of Measurement
        {
            get;
            set;
        }
        public bool bRunning   //whether the Rx is under measurement
        {
            get;
            set;
        }
    }

    //Information related to Rx
    public class RxInfo : IComparable
    {
        //for sorting of Rx according to channel number, and enabled channel goes first.
        public int CompareTo(object obj)
        {
            if (obj is RxInfo)
            {
                RxInfo rx = obj as RxInfo;
                if (this.bEnabled && !rx.bEnabled)
                    return 1;
                if (!this.bEnabled && rx.bEnabled)
                    return -1;

                if (this.cChan < rx.cChan)
                    return -1;
                if (this.cChan > rx.cChan)
                    return 1;
                return 0;
            }
            return 1;
        }
        private SqlConfig config;
        public RX_VAR var;
        public RsInfo Rstd;
        public RxInfo(int id)
        {
            config = new SqlConfig("RxInfo", id.ToString());
            var = new RX_VAR(this);
        }
        public double dRxInput  //target rx value
        {
            get
            {
                try
                {
                    return (double)this["dRxInput"];
                }
                catch
                {
                    return 0.000001;
                }
            }
            set
            {
                this["dRxInput"] = value;
            }
        }

        public double dCoefA  //Coeficient A
        {
            get
            {
                try
                {
                    return (double)this["dCoefA"];
                }
                catch
                {
                    return 0.000001;
                }
            }
            set
            {
                this["dCoefA"] = value;
            }
        }
        public double dCoefB  //Coeficient B
        {
            get
            {
                try
                {
                    return (double)this["dCoefB"];
                }
                catch
                {
                    return 0.000001;
                }
            }
            set
            {
                this["dCoefB"] = value;
            }
        }
        public double dCoefC  //Coeficient C
        {
            get
            {
                try
                {
                    return (double)this["dCoefC"];
                }
                catch
                {
                    return 0.000001;
                }
            }
            set
            {
                this["dCoefC"] = value;
            }
        }
        public int iMeasDelay   //delay between each voltage reading (second)
        {
            get
            {
                return (int)this["iMeasDelay"];
            }
            set
            {
                this["iMeasDelay"] = value;
            }
        }
        public int iMeasTimes   //total measurement times
        {
            get
            {
                return (int)this["iMeasTimes"];
            }
            set
            {
                this["iMeasTimes"] = value;
            }
        }
        public int iIx  //index of current setting.
        {
            get
            {
                return (int)this["iIx"];
            }
            set
            {
                this["iIx"] = value;
            }
        }
        public int iRRange  //index of range for resistance
        {
            get
            {
                return (int)this["iRRange"];
            }
            set
            {
                this["iRRange"] = value;
            }
        }
        public char cChan
        {
            get
            {
                return (char)this["cChan"];
            }
            set
            {
                this["cChan"] = value;
            }
        }
        public bool bEnabled
        {
            get
            {
                return (bool)this["bEnabled"];
            }
            set
            {
                this["bEnabled"] = value;
            }
        }
        public bool bSqrt
        {
            get
            {
                return (bool)this["bSqrt"];
            }
            set
            {
                this["bSqrt"] = value;
            }
        }
        public string sSerial
        {
            get
            {
                return (string)this["sSerial"];
            }
            set
            {
                this["sSerial"] = value;
            }
        }
        public string sPTType
        {
            get
            {
                return (string)this["sPTType"];
            }
            set
            {
                this["sPTType"] = value;
            }
        }        
        public RXDATA_MODE iMainData    //main display info
        {
            get
            {
                return (RXDATA_MODE)this["iMainData"];
            }
            set
            {
                this["iMainData"] = (int)value;
            }
        }
        public RXDATA_MODE iAux1Data    //aux display info
        {
            get
            {
                return (RXDATA_MODE)this["iAux1Data"];
            }
            set
            {
                this["iAux1Data"] = (int)value;
            }
        }
        /*
         * Fixed configuration includes:
         * iMeasDelay :  delay between each sample (second)
         * iMeasTimes :  measure times
         * iChan      :  my channel index
         * cChan      :  A/B of my channel
         * bEnabled   :  whether it is enabled
         * iIx        :  current to use
         * bSqrt      :  sqrt function enabled or not
         * sSerial    :  serial number to use
         * iMainData  :  main display info
         * iAux1Data  :  1st aux display info
         * dCoefA,B,C :  for temperature conversion
         * sPTType    :  PT value
         */
        public object this[string id]
        {
            get
            {
                return config[id];
            }
            set
            {
                config[id] = value;
            }
        }
    }
    public class RsInfo
    {
        private SqlConfig config;
        
        public RsInfo(int id)
        {
            config = new SqlConfig("RsInfo", id.ToString());
        }
        
        public int iRRange
        {
            get
            {
                return (int)this["iRRange"];
            }
            set
            {
                this["iRRange"] = value;
            }
        }
        public double dTValue   //adjusted value based on temperature
        {
            get
            {
                return dValue * (1 + (1e-6) * dAlpha * (RunWnd.syscfg.dTemp - 20) + (1e-6) * dBeta * (RunWnd.syscfg.dTemp - 20) * (RunWnd.syscfg.dTemp - 20)); 
            }
        }
        public double dValue    //nominal value
        {
            get
            {
                return (double)this["dValue"];
            }
            set
            {
                this["dValue"] = value;
            }
        }
        public double dMaxCurr    //Maximum current
        {
            get
            {
                return (double)this["dMaxCurrent"];
            }
            set
            {
                this["dMaxCurrent"] = value;
            }
        }
        public double dAlpha    //temperature coefficient of alpha
        {
            get
            {
                return (double)this["dAlpha"];
            }
            set
            {
                this["dAlpha"] = value;
            }
        }
        public double dBeta     //temperature coefficient of beta
        {
            get
            {
                return (double)this["dBeta"];
            }
            set
            {
                this["dBeta"] = value;
            }
        }
        public string sSerial
        {
            get
            {
                return (string)this["sSerial"];
            }
            set
            {
                this["sSerial"] = value;
            }
        }

        /*  Fixed Configuration
         * dValue     : Rs value
         * dAlpha     : Alpha value
         * dBeta      : Beta value
         * sSerial    : Serial number
         * dMaxCurrent: maximum current value     
         */
        public object this[string id]
        {
            get
            {
                return config[id];
            }
            set
            {
                config[id] = value;
            }
        }
    }
    //variable for display setting configuration
    public class UIConfig
    {
        private SqlConfig config;

        public UIConfig(int disp)
        {
            config = new SqlConfig("UIInfo", disp.ToString());
        }

        public RXDISP_MODE iDataMode    //current display mode
        {
            get
            {
                return (RXDISP_MODE)this["iDataMode"];
            }
            set
            {
                this["iDataMode"] = (int)value;
            }
        }
        public string sChannel  //current top channel display
        {
            get
            {
                return (string)this["sChannel"];
            }
            set
            {
                this["sChannel"] = (string)value;
            }
        }
        public object this[string id]
        {
            get
            {
                return config[id];
            }
            set
            {
                config[id] = value;
            }
        }
    }
    /*
     * Define system configuration
     */
    public class SysConfig
    {
        private SqlConfig config;

        public SysConfig()
        {
            config = new SqlConfig("SysInfo", "pc");
        }
        public int iMeasTimes
        {
            get
            {
                return (int)this["iMeasTimes"];
            }
            set
            {
                this["iMeasTimes"] = value;
            }
        }
        public int iShiftTimes
        {
            get
            {
                return (int)this["iShiftTimes"];
            }
            set
            {
                this["iShiftTimes"] = value;
            }
        }
        public int iSampleTimes
        {
            get
            {
                return (int)this["iSampleTimes"];
            }
            set
            {
                this["iSampleTimes"] = value;
            }
        }
        public int iMeasDelay
        {
            get
            {
                return (int)this["iMeasDelay"];
            }
            set
            {
                this["iMeasDelay"] = value;
            }
        }
        public int iFilter
        {
            get
            {
                return (int)this["iFilter"];
            }
            set
            {
                this["iFilter"] = value;
            }
        }
        public double dTemp
        {
            get
            {
                double val = 0;
                try
                {

                    val = double.Parse(this["dTemp"].ToString());
                }
                catch
                {
                    
                }
                return val;
            }
            set
            {
                this["dTemp"] = value;
            }
        }
        public string sNavmeter
        {
            get
            {
                string nav = (string)this["sNavmeter"];
                if (nav == "")
                    nav = "PZ158";
                return nav;
            }
            set
            {
                this["sNavmeter"] = value;
            }
        }
        public string sFilterType
        {
            get
            {
                return (string)this["sFilterType"];
            }
            set
            {
                this["sFilterType"] = value;
            }
        }
        public string sConvertMethod
        {
            get
            {
                return (string)this["sConvertMethod"];
            }
            set
            {
                this["sConvertMethod"] = value;
            }
        }        
        public int iDisplay
        {
            get
            {
                return (int)this["iDisplay"];
            }
            set
            {
                this["iDisplay"] = value;
            }
        }
        public int iKTT
        {
            get
            {
                return ((int)this["iKTT"]);
            }
            set
            {
                
                this["iKTT"] = value;
            }
        }
        public bool bThrow
        {
            get
            {
                return (1 == ((int)this["iThrow"]));
            }
            set
            {
                if(value)
                    this["iThrow"] = 1;
                else
                    this["iThrow"] = 0;
            }
        }

        /*  Fixed Configuration
         * iSampleTimes : times per sample
         * iMeasDelay : delay per sample
         * iFilter      : filter length
         * sFilterType  : filter type
         * iDisplay     : display mode
         */
        
        public object this[string id]
        {
            get
            {
                return config[id];
            }
            set
            {
                config[id] = value;
            }
        }
        
    }
    /*
     * Manage Sqlite based config file
     * All configure table has following structure 
     * id value group table
     * for current configuration id='current' grp='current'
     */
    internal class SqlConfig
    {
        static private SQLiteConnection sql_con;
        static private SQLiteCommand sql_cmd;

        private SQLiteDataAdapter DB;
        private DataSet DS = new DataSet();
        private DataTable DT = new DataTable();

        private string sql_tbl;
        private string sql_grp;
        static void SetConnection()
        {
            sql_con = new SQLiteConnection("Data Source="+Path.Combine(StringResource.str("basedir"),"Config.db2")+";Version=3;New=False;Compress=True;");
        }
        private Dictionary<string, string> curr_conf; //store all the configuration string
        public SqlConfig(string sql_tbl,string group)
        {
            curr_conf = new Dictionary<string, string>();
            this.sql_tbl = sql_tbl;
            this.sql_grp = group;
            LoadConfigFromFile();
        }

        private bool LoadConfigFromFile()
        {
            try
            {
                string CommandText;
                SetConnection();
                sql_con.Open();

                //retrieve current configure
                CommandText = "select id,value from data where grp='"+this.sql_grp+"' and tbl='" + this.sql_tbl + "'";
                DB = new SQLiteDataAdapter(CommandText, sql_con);
                DS.Reset();
                DB.Fill(DS);
                foreach (DataRow dr in DS.Tables[0].Rows)
                {
                        curr_conf[dr[0].ToString()] = dr[1].ToString();
                }
                sql_con.Close();
                return true;
            }
            catch (System.Exception e)
            {
//              Program.MsgShow(e.Message);
                return false;
            }

        }
        private bool SaveConfigToFile(string id, string val)
        {
            try
            {
                //save the current configuration 
                //SetConnection();
                sql_con.Open();
                sql_cmd = new SQLiteCommand();
                sql_cmd.Connection = sql_con;
                sql_cmd.CommandText = "replace into data(id,value,grp,tbl) values('"+id+"','" + val + "','"+this.sql_grp+"','" + this.sql_tbl + "')";
                sql_cmd.ExecuteNonQuery();
                sql_con.Close();
                return true;
            }
            catch (System.Exception e)
            {
//                Program.MsgShow(e.Message);
                return false;
            }
        }
        public IEnumerable<String> Keys
        {
            get
            {
                return curr_conf.Keys;
            }
        }
        public object this[string id]
        {
            get
            {
                string val = "";
                if (curr_conf.ContainsKey(id))
                    val = curr_conf[id];

                if (id.StartsWith("b")) //bool 
                    return (val == bool.TrueString);
                if (id.StartsWith("c")) //char
                {
                    if (val.Length > 1)
                        return val[0];
                    else
                        return " ";
                }
                if (id.StartsWith("i")) //int
                {
                    if (val == "")
                        return 0;
                    else
                        return Convert.ToInt32(val);
                }
                if (id.StartsWith("d")) //double
                {
                    if (val == "")
                        return 0;
                    else
                        return Convert.ToDouble(val);
                }
                return val;
            }
            set
            {
                string val;
                if (value != null)
                    val = value.ToString();
                else
                    val = "";
                if (curr_conf.ContainsKey(id) && (curr_conf[id]==val))
                    return;
                if(SaveConfigToFile(id,val))
                    curr_conf[id] = val;
            }
        }
        
    }

}
