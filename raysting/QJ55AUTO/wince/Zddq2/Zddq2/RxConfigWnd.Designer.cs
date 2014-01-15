﻿namespace Zddq2
{
    partial class RxConfigWnd
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.lbl_stdchan = new System.Windows.Forms.Label();
            this.lbl_current = new System.Windows.Forms.Label();
            this.lbl_serial = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.lbl_vname = new System.Windows.Forms.Label();
            this.lbl_vmode = new System.Windows.Forms.Label();
            this.btn_next = new Zddq2.RectButton();
            this.btn_last = new Zddq2.RectButton();
            this.btn_enable = new Zddq2.RectButton();
            this.btn_chan = new Zddq2.RectButton();
            this.btn_vname = new Zddq2.RectButton();
            this.btn_vmode = new Zddq2.RectButton();
            this.btn_sqrt = new Zddq2.RectButton();
            this.btn_current = new Zddq2.RectButton();
            this.btn_serial = new Zddq2.RectButton();
            this.btn_stdchan = new Zddq2.RectButton();
            this.btn_SysConfig = new Zddq2.RectButton();
            this.btn_RxConfig = new Zddq2.RectButton();
            this.btn_RsConfig = new Zddq2.RectButton();
            this.btn_quit = new Zddq2.RectButton();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // lbl_stdchan
            // 
            this.lbl_stdchan.Font = new System.Drawing.Font("Arial", 24F, System.Drawing.FontStyle.Regular);
            this.lbl_stdchan.Location = new System.Drawing.Point(553, 208);
            this.lbl_stdchan.Name = "lbl_stdchan";
            this.lbl_stdchan.Size = new System.Drawing.Size(170, 49);
            this.lbl_stdchan.Text = "lbl_stdchan";
            // 
            // lbl_current
            // 
            this.lbl_current.Font = new System.Drawing.Font("Arial", 24F, System.Drawing.FontStyle.Regular);
            this.lbl_current.Location = new System.Drawing.Point(22, 208);
            this.lbl_current.Name = "lbl_current";
            this.lbl_current.Size = new System.Drawing.Size(174, 39);
            this.lbl_current.Text = "lbl_current";
            // 
            // lbl_serial
            // 
            this.lbl_serial.Font = new System.Drawing.Font("Arial", 24F, System.Drawing.FontStyle.Regular);
            this.lbl_serial.Location = new System.Drawing.Point(553, 104);
            this.lbl_serial.Name = "lbl_serial";
            this.lbl_serial.Size = new System.Drawing.Size(170, 45);
            this.lbl_serial.Text = "lbl_serial";
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.White;
            this.panel1.Controls.Add(this.btn_next);
            this.panel1.Controls.Add(this.btn_last);
            this.panel1.Controls.Add(this.btn_enable);
            this.panel1.Controls.Add(this.lbl_vname);
            this.panel1.Controls.Add(this.lbl_vmode);
            this.panel1.Controls.Add(this.lbl_current);
            this.panel1.Controls.Add(this.lbl_serial);
            this.panel1.Controls.Add(this.lbl_stdchan);
            this.panel1.Controls.Add(this.btn_chan);
            this.panel1.Controls.Add(this.btn_vname);
            this.panel1.Controls.Add(this.btn_vmode);
            this.panel1.Controls.Add(this.btn_sqrt);
            this.panel1.Controls.Add(this.btn_current);
            this.panel1.Controls.Add(this.btn_serial);
            this.panel1.Controls.Add(this.btn_stdchan);
            this.panel1.Location = new System.Drawing.Point(12, 88);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(994, 504);
            this.panel1.GotFocus += new System.EventHandler(this.panel1_GotFocus);
            // 
            // lbl_vname
            // 
            this.lbl_vname.Font = new System.Drawing.Font("Arial", 24F, System.Drawing.FontStyle.Regular);
            this.lbl_vname.Location = new System.Drawing.Point(22, 104);
            this.lbl_vname.Name = "lbl_vname";
            this.lbl_vname.Size = new System.Drawing.Size(174, 45);
            this.lbl_vname.Text = "lbl_vname";
            // 
            // lbl_vmode
            // 
            this.lbl_vmode.Font = new System.Drawing.Font("Arial", 24F, System.Drawing.FontStyle.Regular);
            this.lbl_vmode.Location = new System.Drawing.Point(22, 318);
            this.lbl_vmode.Name = "lbl_vmode";
            this.lbl_vmode.Size = new System.Drawing.Size(174, 36);
            this.lbl_vmode.Text = "lbl_vmode";
            // 
            // btn_next
            // 
            this.btn_next.BackColor = System.Drawing.Color.Red;
            this.btn_next.bEnabled = true;
            this.btn_next.bOn = true;
            this.btn_next.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold);
            this.btn_next.Location = new System.Drawing.Point(304, 20);
            this.btn_next.Name = "btn_next";
            this.btn_next.Size = new System.Drawing.Size(59, 46);
            this.btn_next.TabIndex = 4;
            // 
            // btn_last
            // 
            this.btn_last.BackColor = System.Drawing.Color.Red;
            this.btn_last.bEnabled = true;
            this.btn_last.bOn = true;
            this.btn_last.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold);
            this.btn_last.Location = new System.Drawing.Point(39, 20);
            this.btn_last.Name = "btn_last";
            this.btn_last.Size = new System.Drawing.Size(59, 46);
            this.btn_last.TabIndex = 4;
            // 
            // btn_enable
            // 
            this.btn_enable.BackColor = System.Drawing.Color.Red;
            this.btn_enable.bEnabled = true;
            this.btn_enable.bOn = true;
            this.btn_enable.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold);
            this.btn_enable.Location = new System.Drawing.Point(553, 20);
            this.btn_enable.Name = "btn_enable";
            this.btn_enable.Size = new System.Drawing.Size(162, 46);
            this.btn_enable.TabIndex = 3;
            // 
            // btn_chan
            // 
            this.btn_chan.BackColor = System.Drawing.Color.Red;
            this.btn_chan.bEnabled = true;
            this.btn_chan.bOn = true;
            this.btn_chan.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold);
            this.btn_chan.Location = new System.Drawing.Point(152, 20);
            this.btn_chan.Name = "btn_chan";
            this.btn_chan.Size = new System.Drawing.Size(107, 46);
            this.btn_chan.TabIndex = 0;
            // 
            // btn_vname
            // 
            this.btn_vname.BackColor = System.Drawing.Color.Red;
            this.btn_vname.bEnabled = true;
            this.btn_vname.bOn = true;
            this.btn_vname.Font = new System.Drawing.Font("Arial", 20F, System.Drawing.FontStyle.Bold);
            this.btn_vname.Location = new System.Drawing.Point(209, 93);
            this.btn_vname.Name = "btn_vname";
            this.btn_vname.Size = new System.Drawing.Size(188, 58);
            this.btn_vname.TabIndex = 0;
            // 
            // btn_vmode
            // 
            this.btn_vmode.BackColor = System.Drawing.Color.Red;
            this.btn_vmode.bEnabled = true;
            this.btn_vmode.bOn = true;
            this.btn_vmode.Font = new System.Drawing.Font("Arial", 20F, System.Drawing.FontStyle.Bold);
            this.btn_vmode.Location = new System.Drawing.Point(209, 306);
            this.btn_vmode.Name = "btn_vmode";
            this.btn_vmode.Size = new System.Drawing.Size(188, 58);
            this.btn_vmode.TabIndex = 0;
            // 
            // btn_sqrt
            // 
            this.btn_sqrt.BackColor = System.Drawing.Color.Red;
            this.btn_sqrt.bEnabled = true;
            this.btn_sqrt.bOn = true;
            this.btn_sqrt.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold);
            this.btn_sqrt.Location = new System.Drawing.Point(328, 197);
            this.btn_sqrt.Name = "btn_sqrt";
            this.btn_sqrt.Size = new System.Drawing.Size(69, 57);
            this.btn_sqrt.TabIndex = 0;
            // 
            // btn_current
            // 
            this.btn_current.BackColor = System.Drawing.Color.Red;
            this.btn_current.bEnabled = true;
            this.btn_current.bOn = true;
            this.btn_current.Font = new System.Drawing.Font("Arial", 20F, System.Drawing.FontStyle.Bold);
            this.btn_current.Location = new System.Drawing.Point(209, 197);
            this.btn_current.Name = "btn_current";
            this.btn_current.Size = new System.Drawing.Size(111, 58);
            this.btn_current.TabIndex = 0;
            // 
            // btn_serial
            // 
            this.btn_serial.BackColor = System.Drawing.Color.Red;
            this.btn_serial.bEnabled = true;
            this.btn_serial.bOn = true;
            this.btn_serial.Font = new System.Drawing.Font("Arial", 20F, System.Drawing.FontStyle.Bold);
            this.btn_serial.Location = new System.Drawing.Point(729, 93);
            this.btn_serial.Name = "btn_serial";
            this.btn_serial.Size = new System.Drawing.Size(188, 58);
            this.btn_serial.TabIndex = 0;
            // 
            // btn_stdchan
            // 
            this.btn_stdchan.BackColor = System.Drawing.Color.Red;
            this.btn_stdchan.bEnabled = true;
            this.btn_stdchan.bOn = true;
            this.btn_stdchan.Font = new System.Drawing.Font("Arial", 20F, System.Drawing.FontStyle.Bold);
            this.btn_stdchan.Location = new System.Drawing.Point(729, 199);
            this.btn_stdchan.Name = "btn_stdchan";
            this.btn_stdchan.Size = new System.Drawing.Size(188, 58);
            this.btn_stdchan.TabIndex = 0;
            // 
            // btn_SysConfig
            // 
            this.btn_SysConfig.BackColor = System.Drawing.Color.Blue;
            this.btn_SysConfig.bEnabled = true;
            this.btn_SysConfig.bOn = true;
            this.btn_SysConfig.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold);
            this.btn_SysConfig.Location = new System.Drawing.Point(12, 12);
            this.btn_SysConfig.Name = "btn_SysConfig";
            this.btn_SysConfig.Size = new System.Drawing.Size(136, 46);
            this.btn_SysConfig.TabIndex = 5;
            // 
            // btn_RxConfig
            // 
            this.btn_RxConfig.BackColor = System.Drawing.Color.Blue;
            this.btn_RxConfig.bEnabled = true;
            this.btn_RxConfig.bOn = true;
            this.btn_RxConfig.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold);
            this.btn_RxConfig.Location = new System.Drawing.Point(316, 12);
            this.btn_RxConfig.Name = "btn_RxConfig";
            this.btn_RxConfig.Size = new System.Drawing.Size(136, 46);
            this.btn_RxConfig.TabIndex = 4;
            // 
            // btn_RsConfig
            // 
            this.btn_RsConfig.BackColor = System.Drawing.Color.Blue;
            this.btn_RsConfig.bEnabled = true;
            this.btn_RsConfig.bOn = true;
            this.btn_RsConfig.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold);
            this.btn_RsConfig.Location = new System.Drawing.Point(164, 12);
            this.btn_RsConfig.Name = "btn_RsConfig";
            this.btn_RsConfig.Size = new System.Drawing.Size(136, 46);
            this.btn_RsConfig.TabIndex = 3;
            // 
            // btn_quit
            // 
            this.btn_quit.BackColor = System.Drawing.Color.Red;
            this.btn_quit.bEnabled = true;
            this.btn_quit.bOn = true;
            this.btn_quit.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold);
            this.btn_quit.Location = new System.Drawing.Point(928, 11);
            this.btn_quit.Name = "btn_quit";
            this.btn_quit.Size = new System.Drawing.Size(71, 71);
            this.btn_quit.TabIndex = 0;
            // 
            // RxConfigWnd
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(1024, 673);
            this.ControlBox = false;
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.btn_SysConfig);
            this.Controls.Add(this.btn_RxConfig);
            this.Controls.Add(this.btn_RsConfig);
            this.Controls.Add(this.btn_quit);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "RxConfigWnd";
            this.Text = "RxConfigWnd";
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private RectButton btn_quit;
        private System.Windows.Forms.Label lbl_stdchan;
        private RectButton btn_stdchan;
        private RectButton btn_current;
        private System.Windows.Forms.Label lbl_current;
        private RectButton btn_sqrt;
        private RectButton btn_chan;
        private System.Windows.Forms.Label lbl_serial;
        private RectButton btn_serial;
        private RectButton btn_SysConfig;
        private RectButton btn_RxConfig;
        private RectButton btn_RsConfig;
        private System.Windows.Forms.Panel panel1;
        private RectButton btn_enable;
        private System.Windows.Forms.Label lbl_vmode;
        private RectButton btn_vmode;
        private RectButton btn_next;
        private RectButton btn_last;
        private System.Windows.Forms.Label lbl_vname;
        private RectButton btn_vname;
    }
}
