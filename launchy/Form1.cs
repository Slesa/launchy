#region License
/*
Launchy: Application Launcher
Copyright (C) 2005  Josh Karlin

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#endregion

#region Includes
using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using Onyeyiri;
using System.Threading;
using System.Timers;
using System.Text;
using System.IO;
using System.Text.RegularExpressions;
//using System;
using System.Diagnostics;
using System.Resources;
using System.Reflection;
using Microsoft.Win32;
using System.Runtime.InteropServices;
#endregion

namespace Launchy
{


	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Launchy : System.Windows.Forms.Form
    {
        #region Boiler
         //API functions to move the form
        public const int WM_NCLBUTTONDOWN = 0xA1;
		public const int HTCAPTION = 0x2;
		[DllImportAttribute("user32.dll")]
		public static extern bool ReleaseCapture();
		[DllImportAttribute("user32.dll")]
		public static extern int SendMessage(IntPtr hWnd, int Msg, int wParam, int lParam);
        #endregion

        #region Privates
        private System.Windows.Forms.ComboBox listBox;
		private HotKeyManager hkm;
		private bool atLaunch;
		private System.Timers.Timer delayTimer;
		private System.Timers.Timer rescanTimer;

        /// <summary>
        /// The catalog of records
        /// </summary>
        private SortedList catalog;
        /// <summary>
        /// The result of the last query
        /// </summary>
		private ArrayList currentMatches;
        private Record selectedRecord;
        private bool ignoreNextChange;
		private System.Windows.Forms.TextBox Suggestion;
        private Launcher exeL = new exeLauncher();
        public static String queryText;

        private System.ComponentModel.Container components = null;
        #endregion

        /// <summary>
        /// At program launch sets the program to not visible
        /// 
        /// Otherwise it toggles visibility
        /// </summary>
        /// <param name="id">not used</param>
        public void Handler(int id) 
		{
			if (atLaunch) { 
				this.Opacity = 100; 
				this.Visible = false;
				atLaunch = false;
			}
			this.Visible = !this.Visible;
			if (this.Visible) 
			{
				this.Activate();
				listBox.SelectAll();
			}			
		}


        /// <summary>
        /// Called when the user has typed in a query but has not hit enter before the timeout, so we should
        /// show a dropdown list of other suggestions.
        /// </summary>
        /// <param name="source">unused</param>
        /// <param name="e">unused</param>
		public  void onTimer(Object source, ElapsedEventArgs e) 
		{
			if (listBox.Text == "") { return; }
			delayTimer.Stop();
			if (this.Visible != true) { return; }
			updateFields(listBox.Text);
	
			if (currentMatches.Count <= 1) { return; }

			listBox.Items.Clear();
		
			listBox.BeginUpdate();
			for(int i = 0; i < currentMatches.Count; i++)
				listBox.Items.Insert(i,currentMatches[i]);

			listBox.EndUpdate();
			
			listBox.DroppedDown = true;
			listBox.Select(listBox.Text.Length, 0);
		}

        /// <summary>
        /// A callback timer function to rescan the catalog.  It simply calls rescan().
        /// </summary>
        /// <param name="source">unused</param>
        /// <param name="e">unused</param>
		public void onRescanTimer(Object source, ElapsedEventArgs e)
		{
			rescan();
		}

        /// <summary>
        /// Emptyes the catalog and refills it with fresh records
        /// </summary>
		private void rescan() 
		{
			catalog.Clear();
			scanStartMenu();
		}

        /// <summary>
        /// Given a directory and a filetype it recursively (infinite depth) scans the
        /// directory and subdirectories for the file type, creates records of them, and
        /// adds them to the catalog
        /// </summary>
        /// <param name="dir">The root of the directory tree to search</param>
        /// <param name="type">The filetype to search for</param>
        /// <param name="launcher">The class which knows how to execute the type</param>
		private void scanMenu(string dir, string type, Launcher launcher) 
		{
			DirectoryInfo di;
			try 
			{
				di = new DirectoryInfo(dir);
				DirectoryInfo[] dirs = di.GetDirectories();
				foreach(DirectoryInfo curDir in dirs) 
				{				
					scanMenu(curDir.FullName, type, launcher);
				}
				FileInfo[] rgFiles = di.GetFiles("*" + type);
				
				foreach(FileInfo fi in rgFiles)
				{
					Record rec = new Record();
					rec.Name = fi.Name;
					rec.LowName = fi.Name.ToLower();
					rec.Path = fi.DirectoryName;

                    rec.type = type;
                    rec.launcher = launcher;

                    // Is it a duplicate?
					bool isdup = false;

                    /*
                    // This is expensive, it's n^2 just to add records
                    // to the catalog.  We need to find a slick way around this.
					foreach(Record r in catalog) 
					{
						if (r.Name == rec.Name) 
							isdup = true;
					}
                    */
                    if (!catalog.ContainsKey(rec.Name))
						catalog.Add(rec.Name, (object)rec);
				}
			}
			catch(Exception e) {return;}
		}
		
        /// <summary>
        /// Automagically detects the locations of the start menu directories and adds their contents to the
        /// catalog by calling scanMenu() on those directories with "*.lnk" as the file type
        /// </summary>
		private void scanStartMenu() 
		{
			string myMenu = Environment.GetFolderPath(Environment.SpecialFolder.StartMenu).ToString();
			string allMenus = Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData).ToString();
			allMenus = allMenus.Trim("Application Data".ToCharArray());
			allMenus = allMenus + "Start Menu";
			scanMenu(myMenu, ".lnk", exeL);
			scanMenu(allMenus, ".lnk",exeL);
            
            // Want mp3 listings?  Do something like this.. it works great!
            // scanMenu("C:\\Documents and Settings\\karlinjf\\My Documents\\My Music", ".mp3", exeL);
		}



        /// <summary>
        /// Does the routine startup stuff such as starting the timers, performing the initial catalog update,
        /// and setting visibility as well as boilerplate GUI code.
        /// </summary>
		public Launchy()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
			catalog = new SortedList(100);
			currentMatches = new ArrayList(100);
			ignoreNextChange = false;


			hkm = new HotKeyManager();
			hkm.Register(Keys.Space, HotKeyModifier.Alt, 100, new HotKeyEventHandler(Handler));

			delayTimer = new System.Timers.Timer();
			delayTimer.Elapsed += new ElapsedEventHandler(onTimer);
			delayTimer.Interval = 1000;
			delayTimer.Stop();
			delayTimer.AutoReset = false;

			rescanTimer = new System.Timers.Timer();
			rescanTimer.Elapsed += new ElapsedEventHandler(onRescanTimer);
			rescanTimer.Interval = 1200000; // 20 minutes
			rescanTimer.Start();
			rescanTimer.AutoReset = true;

			rescan();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
			this.Opacity = 0;
			this.Visible = false;
			atLaunch = true;

			this.MouseDown += new MouseEventHandler(this.Form1_MouseDown);
		}

        /// <summary>
        /// Application shutdown.  Save the XY coordinates of the program to the registry.
        /// </summary>
        /// <param name="disposing"></param>
		protected override void Dispose( bool disposing )
		{
			// Open the key
			RegistryKey key = Registry.CurrentUser.OpenSubKey( "Software\\Sprout\\WindowPos", true );

			// Set the registry values to correspond to the form's coordinates on the
			// screen.
			key.SetValue( "X", Location.X );
			key.SetValue( "Y", Location.Y ); 

			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}

			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.listBox = new System.Windows.Forms.ComboBox();
            this.Suggestion = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
// 
// listBox
// 
            this.listBox.BackColor = System.Drawing.Color.BlanchedAlmond;
            this.listBox.CausesValidation = false;
            this.listBox.Font = new System.Drawing.Font("Trebuchet MS", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.listBox.ForeColor = System.Drawing.Color.RoyalBlue;
            this.listBox.FormattingEnabled = true;
            this.listBox.Location = new System.Drawing.Point(26, 23);
            this.listBox.Name = "listBox";
            this.listBox.Size = new System.Drawing.Size(176, 26);
            this.listBox.TabIndex = 2;
            this.listBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.listBox_KeyPress);
            this.listBox.TextChanged += new System.EventHandler(this.listBox_TextChanged);
// 
// Suggestion
// 
            this.Suggestion.AutoSize = false;
            this.Suggestion.BackColor = System.Drawing.Color.BlanchedAlmond;
            this.Suggestion.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.Suggestion.Font = new System.Drawing.Font("Trebuchet MS", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Suggestion.ForeColor = System.Drawing.Color.RoyalBlue;
            this.Suggestion.Location = new System.Drawing.Point(240, 25);
            this.Suggestion.Name = "Suggestion";
            this.Suggestion.ReadOnly = true;
            this.Suggestion.Size = new System.Drawing.Size(177, 22);
            this.Suggestion.TabIndex = 5;
            this.Suggestion.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.Suggestion.WordWrap = false;
// 
// Form1
// 
            this.AutoScaleBaseSize = new System.Drawing.Size(6, 17);
            this.BackColor = System.Drawing.Color.Red;
            this.ClientSize = new System.Drawing.Size(442, 72);
            this.ControlBox = false;
            this.Controls.Add(this.Suggestion);
            this.Controls.Add(this.listBox);
            this.Font = new System.Drawing.Font("Comic Sans MS", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ForeColor = System.Drawing.Color.Coral;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form1";
            this.ShowInTaskbar = false;
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Launchy";
            this.TopMost = true;
            this.TransparencyKey = System.Drawing.Color.Red;
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);

        }
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Launchy());
		}

        /// <summary>
        /// Setup the background image, enable transparency, and move the window position
        /// to that saved in the registry if available
        /// </summary>
        /// <param name="sender">unused</param>
        /// <param name="e">unused</param>
		private void Form1_Load(object sender, System.EventArgs e)
		{
			try 
			{
				Stream imgStream = null;
				Bitmap bmp = null;      
				Assembly a = Assembly.GetExecutingAssembly();

				// get a list of resource names from the manifest
				string [] resNames = a.GetManifestResourceNames();

				foreach(string s in resNames)
				{
					if(s.EndsWith("bkgd2.bmp"))
					{
						// attach to stream to the resource in the manifest
						imgStream = a.GetManifestResourceStream(s);
						if( !(null==imgStream) )
						{                    
							// create a new bitmap from this stream and 
							// add it to the arraylist
							bmp = Bitmap.FromStream( imgStream ) as Bitmap;
						//	imgStream.Close();
						//	imgStream = null;
						}
					}
				}            		
		
				if (bmp != null) 
				{
                    bmp.MakeTransparent(bmp.GetPixel(0,0));
					this.BackgroundImage = bmp;
					//this.TransparencyKey = bmp.GetPixel(0,0);
				}
				if (imgStream != null) { imgStream.Close(); }
			} 
			catch (Exception ex) {MessageBox.Show(ex.ToString());}

			// Set the window position

			// Attempt to open the key
			RegistryKey key = Registry.CurrentUser.OpenSubKey( "Software\\Sprout\\WindowPos" );

			// If the return value is null, the key doesn't exist
			if ( key == null ) 
			{
				// The key doesn't exist; create it / open it
				key = Registry.CurrentUser.CreateSubKey( "Software\\Sprout\\WindowPos" );
			}

			// Attempt to retrieve the value X; if null is returned, the value
			// doesn't exist in the registry.
			if ( key.GetValue( "X" ) != null ) 
			{
				// The value exists; move the form to the coordinates stored in the
				// registry.
				Location = new Point( (int)key.GetValue( "X" ), (int)key.GetValue( "Y" ) );
			}
		}

        /// <summary>
        /// Launch the currently selected record
        /// </summary>
		private void Execute() 
		{
            selectedRecord.launcher.Launch(selectedRecord);	
		}




        /// <summary>
        /// When it's time to find the best matches for the users query, this function gets called.
        /// 
        /// It clears the current list of matches, then it finds all records in the catalog that match
        /// the loose general expression form of the user's query, then it sorts the matching records by 
        /// calling sortMatches, and finally it places the #1 suggestion in the suggestion box found in the GUI
        /// </summary>
        /// <param name="text">The user's query</param>
		private void updateFields(string text) 
		{
			//			MessageBox.Show(text)
			// Find the records that match
			currentMatches.Clear();
			string regex = "^.*";
			foreach (char c in text) 
			{
				regex += c + ".*"; 
			}
			string lowtext = text.ToLower();
			// Compile the regular expression.
			Regex r = new Regex(regex, RegexOptions.IgnoreCase);

			Match m;
            for (int i = 0; i < catalog.Count; i++)
            {
                Record rec = (Record)catalog.GetByIndex(i);
                m = r.Match(rec.Name);
                if (m.Success)
                {
                    rec.match = m;
                    currentMatches.Add(rec);
                }
            }

			// Sort the matches
        
            queryText = lowtext;
            currentMatches.Sort();
		//	sortMatches(lowtext);

			if (currentMatches.Count > 0) 
			{
                String best = ((Record)currentMatches[0]).ToString();
					Suggestion.Text = best;
//				Suggestion.Text = ((Record) currentMatches[0]).Name.TrimEnd(".lnk".ToCharArray());
                selectedRecord = (Record)currentMatches[0];
            }
		}


        /// <summary>
        /// We need to register all keystrokes, and this is the function to do it.
        /// Escape key makes the program dissapear
        /// Return key causes execution
        /// Keys which confuse the regular expression matcher such as ( * and . are ignored
        /// 
        /// </summary>
        /// <param name="sender">unused</param>
        /// <param name="e">unused</param>
		private void listBox_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
		{
			delayTimer.Stop();
			delayTimer.Start();
			listBox.DroppedDown = false;

			if (e.KeyChar == 27) 
			{
				// escape key
				currentMatches.Clear();
				delayTimer.Stop();
				this.Visible = false;
			}
			if (e.KeyChar == '\r') 
			{
				updateFields(listBox.Text);
				this.Visible = false;
				Execute();
				e.Handled = true;
				return;
			}
			if (e.KeyChar == ')' || e.KeyChar == '(' || e.KeyChar == '*' || e.KeyChar == '.') 
			{
				e.Handled = true;
				return;
			}
			
		}

        /// <summary>
        /// If listbox_KeyPress doesn't know what to do with the key press then it's going to be
        /// part of the user's query, update matches based upon the new query.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
		private void listBox_TextChanged(object sender, System.EventArgs e)
		{
			if (listBox.Text == "") 
			{
				currentMatches.Clear();
				delayTimer.Stop();
			}
			else 
			{
				updateFields(listBox.Text);
			}
		
		}



		public void Form1_MouseDown(object sender, MouseEventArgs e)
		{
			//If the left mouse is pressed, release form for movement
			if (e.Button == MouseButtons.Left)
			{
				ReleaseCapture();
				SendMessage(Handle, WM_NCLBUTTONDOWN, HTCAPTION, 0);
			}
		}


	}

    public abstract class Launcher
    {
        public abstract void Launch(Record rec);
    }


    public class exeLauncher : Launcher
    {
        public  override void Launch(Record rec)
        {
            String execPath = rec.Path + "\\" + rec.Name;
            try
            {
                Process exec = new Process();
                exec.StartInfo.FileName = execPath;
                exec.StartInfo.Arguments = "";
                exec.StartInfo.UseShellExecute = true;

                exec.Start();
            }
            catch (Exception e) { }
        }
    }


    public class Record : IComparable
	{
        /// <summary>
        /// The filename
        /// </summary>
		public String Name;
        
        /// <summary>
        /// The lowercase version of the filename.  Stored this way so that it only has to be done once.
        /// </summary>
		public String LowName;
        
        /// <summary>
        /// The path to the file
        /// </summary>
		public String Path;

        /// <summary>
        /// The file extension that was used to match this file
        /// </summary>
		public String type;

        /// <summary>
        /// The class which knows how to execute this type of file
        /// </summary>
        public Launcher launcher;

        public Match match;

        /// <summary>
        /// Returns the filename without the type attachment at the end
        /// </summary>
        /// <returns></returns>
		public override string ToString() 
		{
            int ind = Name.IndexOf(type);
			if (ind != -1)
				return Name.Substring(0,ind);
			else
				return Name;
		}


       /// <summary>
       /// This CompareTo is used for a query.  The catalog is kept sorted via
       /// the usual String.CompareTo
       /// We sort based on two variables.  We take preference to strings which have exact substring matches
       /// with the query.  Otherwise, we sort by string length, shorter is better.
       /// </summary>
       /// <param name="x">the other record to compare to</param>
       /// <returns>1 if 'this' is greater than x
       ///          -1 if 'this' is less than x
       ///          0 if tied</returns>

        public int CompareTo(object x) {
            Record y = (Record) x;

            if (y.LowName.IndexOf(Launchy.queryText) > -1 && this.LowName.IndexOf(Launchy.queryText) == -1)
			{
                return 1;
			}
            if (this.LowName.IndexOf(Launchy.queryText) > -1 && y.LowName.IndexOf(Launchy.queryText) == -1)
            {
                return -1;
            }
            if (this.Name.Length > y.Name.Length)
            {
                return 1;
            }
            if (this.Name.Length <= y.Name.Length)
            {
                return -1;
            }
            
            return 0;
        }
	}
}
