﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Xml.Linq;
using System.IO;
using System.Threading;
namespace ioex_cs
{
    /// <summary>
    /// Interaction logic for Logon.xaml
    /// </summary>
    public partial class LogonWindow : Window
    {
        private Thread bootup;
        System.Windows.Forms.Timer uiTimer;
        private bool boot_ok;
        public LogonWindow()
        {

            InitializeComponent();
            bootup = new Thread(PackerBoot);
            bootup.IsBackground = true;
            uiTimer = new System.Windows.Forms.Timer();
            uiTimer.Tick += new EventHandler(uiTimer_Tick);
            uiTimer.Interval = 200;
            
        
        }
        void uiTimer_Tick(object sender, EventArgs e)
        {
            if (bootup.IsAlive)
                return;
            uiTimer.Stop();
            if (boot_ok == false)
            {
                App p = Application.Current as App;
                foreach (Window w in p.Windows)
                    w.Close();
                p.Shutdown();

            }
            else
            {
                App p = (Application.Current as App);
                p.SwitchTo("runmode");
                this.Visibility = Visibility.Hidden;

            }
        }
        public void PackerBoot()
        {
            App p = (Application.Current as App);
            try
            {
                //search for nodes
                UpdateMessage(StringResource.str("search_newnode"));
                p.agent.missingnode["addr"] = null;
                
                //check the availability of each board
                foreach (SubNode n in p.agent.nodemap.Values)
                {
                    UpdateMessage(StringResource.str("search_node") + n.node_id);
                    if (p.agent.search(n))
                        UpdateMessage(StringResource.str("found_node") + n.node_id);
                    else
                        UpdateMessage(StringResource.str("fail"));
                }
                //start engine
                p.agent.Start();
                //load config for node
                UpdateMessage(StringResource.str("init_nodereg"));
                for (int i = 0; i < p.packers.Count; i++)
                {
                    p.packers[i].InitConfig();
                }
                boot_ok = true;
                p.agent.bBootDone = true;
            }
            catch (Exception ex)
            {
                UpdateMessage(ex.Message);
                Thread.Sleep(3000);
            }
        }
        public void UpdateMessage(string msg)
        {
            Dispatcher.Invoke(System.Windows.Threading.DispatcherPriority.Background, (Action)delegate
            {
                bootmsg.AppendText(msg + "\r\n");
                bootmsg.ScrollToEnd();
            });
            
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            (Application.Current as App).InitAll();
            boot_ok = false;
            bootup.Start();
            uiTimer.Start();
        }
    }
    public class StringResource
    {
        private static Dictionary<string, string> str_tbl;
        static public string language;
        static public void SetLanguage(string lang)
        {
            language = lang;
            XDocument xml_doc;
            xml_doc = XDocument.Load("Resources\\Lang\\"+lang+".xaml");
            IEnumerable<XElement> elem = xml_doc.Elements().First().Elements();
            str_tbl.Clear();
            foreach (XElement e in elem)
            {

                str_tbl[e.FirstAttribute.Value] = e.Value.ToString();
            }
            App p = Application.Current as App;
            p.Resources.Source = new Uri("pack://siteOfOrigin:,,,/Resources/Lang/"+lang+".xaml");
        }
        static StringResource()
        {
            str_tbl = new Dictionary<string, string>();
            FileStream fsLog = new FileStream(System.Threading.Thread.GetDomain().BaseDirectory + "\\history.log", FileMode.Truncate, FileAccess.Write, FileShare.Read);
            fsLog.Close();

            SetLanguage("zh-CN");
        }
        static public void dolog(string log)
        {
            try
            {
                FileStream fsLog = new FileStream(System.Threading.Thread.GetDomain().BaseDirectory + "\\history.log", FileMode.Append, FileAccess.Write, FileShare.Read);
                StreamWriter sw = new StreamWriter(fsLog);
                sw.WriteLine(DateTime.Now.ToString("G") + "\t" + log);
                sw.Close();
                fsLog.Close();
            }
            catch (System.Exception)
            {

            }

        }

        public static string str(string key)
        {
            if (str_tbl.ContainsKey(key))
            {
                return str_tbl[key];
            }else
            {
                return "Invalid String Key";
            }
            
        }
        private StringResource(){}

    }

    public class Password
    {
        private static XmlConfig pwds;
        
        static Password()
        {
            //load password.xml and fill in the username list
            pwds = new XmlConfig("password.xml");
            pwds.LoadConfigFromFile();
        }
        public static IEnumerable<string> users {
            get{
                return pwds.Keys;
            }
        }
        public static string get_pwd(string user){
                XElement x = pwds.LoadConfig(user);
                if (null == x)
                    return pwds.GetHashCode().ToString(); //just a difficult password to guess
                else
                    return x.Value;
            }
        public static void set_pwd(string user,string value){
                XElement x = new XElement("Item");
                x.SetAttributeValue("Name", user);
                x.SetValue(value);
                pwds.AddConfig(user,x);
                pwds.SaveConfigToFile();
            
        }
    }

}