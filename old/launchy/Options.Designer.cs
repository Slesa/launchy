namespace Launchy
{
    partial class Options
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
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
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.butAdd = new System.Windows.Forms.Button();
            this.butBrowse = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.textExt = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.textPath = new System.Windows.Forms.TextBox();
            this.butRemove = new System.Windows.Forms.Button();
            this.listDirs = new System.Windows.Forms.ListBox();
            this.browseDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.butOkay = new System.Windows.Forms.Button();
            this.tabControl1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.SuspendLayout();
// 
// tabControl1
// 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(13, 13);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(465, 269);
            this.tabControl1.TabIndex = 0;
// 
// tabPage1
// 
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(457, 243);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "General";
// 
// tabPage2
// 
            this.tabPage2.Controls.Add(this.butAdd);
            this.tabPage2.Controls.Add(this.butBrowse);
            this.tabPage2.Controls.Add(this.label2);
            this.tabPage2.Controls.Add(this.textExt);
            this.tabPage2.Controls.Add(this.label1);
            this.tabPage2.Controls.Add(this.textPath);
            this.tabPage2.Controls.Add(this.butRemove);
            this.tabPage2.Controls.Add(this.listDirs);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(457, 243);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Advanced";
// 
// butAdd
// 
            this.butAdd.Location = new System.Drawing.Point(293, 163);
            this.butAdd.Name = "butAdd";
            this.butAdd.TabIndex = 7;
            this.butAdd.Text = "Add";
            this.butAdd.Click += new System.EventHandler(this.butAdd_Click);
// 
// butBrowse
// 
            this.butBrowse.Location = new System.Drawing.Point(173, 164);
            this.butBrowse.Name = "butBrowse";
            this.butBrowse.TabIndex = 6;
            this.butBrowse.Text = "Browse";
            this.butBrowse.Click += new System.EventHandler(this.butBrowse_Click);
// 
// label2
// 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(269, 116);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(81, 14);
            this.label2.TabIndex = 5;
            this.label2.Text = "File Extensions";
// 
// textExt
// 
            this.textExt.Location = new System.Drawing.Point(269, 137);
            this.textExt.Name = "textExt";
            this.textExt.TabIndex = 4;
// 
// label1
// 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 116);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(82, 14);
            this.label1.TabIndex = 3;
            this.label1.Text = "Add a Directory";
// 
// textPath
// 
            this.textPath.Location = new System.Drawing.Point(7, 137);
            this.textPath.Name = "textPath";
            this.textPath.Size = new System.Drawing.Size(241, 20);
            this.textPath.TabIndex = 2;
// 
// butRemove
// 
            this.butRemove.Location = new System.Drawing.Point(376, 44);
            this.butRemove.Name = "butRemove";
            this.butRemove.TabIndex = 1;
            this.butRemove.Text = "Remove";
            this.butRemove.Click += new System.EventHandler(this.butRemove_Click);
// 
// listDirs
// 
            this.listDirs.FormattingEnabled = true;
            this.listDirs.Location = new System.Drawing.Point(7, 7);
            this.listDirs.Name = "listDirs";
            this.listDirs.Size = new System.Drawing.Size(362, 95);
            this.listDirs.TabIndex = 0;
// 
// butOkay
// 
            this.butOkay.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.butOkay.Location = new System.Drawing.Point(403, 289);
            this.butOkay.Name = "butOkay";
            this.butOkay.TabIndex = 1;
            this.butOkay.Text = "Okay";
            this.butOkay.Click += new System.EventHandler(this.butOkay_Click);
// 
// Options
// 
            this.AcceptButton = this.butOkay;
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(493, 330);
            this.ControlBox = false;
            this.Controls.Add(this.butOkay);
            this.Controls.Add(this.tabControl1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "Options";
            this.Text = "Form2";
            this.Load += new System.EventHandler(this.Options_Load);
            this.tabControl1.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.ListBox listDirs;
        private System.Windows.Forms.Button butAdd;
        private System.Windows.Forms.Button butBrowse;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textExt;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textPath;
        private System.Windows.Forms.Button butRemove;
        private System.Windows.Forms.FolderBrowserDialog browseDialog;
        private System.Windows.Forms.Button butOkay;
    }
}