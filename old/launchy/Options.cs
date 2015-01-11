#region Using directives

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using Microsoft.Win32;
using System.Runtime.InteropServices;


#endregion

namespace Launchy
{
    partial class Options : Form
    {
        public List<launchyDir> dirs;
        public int posX;
        public int posY;

        public Options()
        {
            InitializeComponent();
            dirs = new List<launchyDir>();

            parseOptions();
        }

        private void parseOptions() {
            // Does the file exist?
            RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\Launchy\\WindowPos");
            if (key == null) { 
                key = Registry.CurrentUser.CreateSubKey("Software\\Launchy\\WindowPos"); 
                posX = 0; posY = 0; 
            }
            else {posX = (int) key.GetValue("X"); posY = (int) key.GetValue("Y");}
            key.Close();

            // Grab the list of user added directories
            key = Registry.CurrentUser.OpenSubKey("Software\\Launchy\\Dirs");
            if (key == null)
            {
                key = Registry.CurrentUser.CreateSubKey("Software\\Launchy\\Dirs");
            }
            int j = 0;
            foreach (string dEntry in key.GetValueNames())
            {
                launchyDir d = new launchyDir();
                d.types = new List<String>();
                d.entryNum = j;
                string entry = (string)key.GetValue(dEntry);
                string[] split = entry.Split("%".ToCharArray());
                d.path = dEntry;
                for (int i = 0; i < split.Length; i++)
                {
                    d.types.Add(split[i]);
                }
                dirs.Add(d);
                listDirs.Items.Add(d);
                j++;
            }
            key.Close();
        }


        public void saveOptions(int X, int Y)
        {
            RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\Launchy\\WindowPos", true);
            key.SetValue("X", X);
            key.SetValue("Y", Y);
            key.Close();
            Registry.CurrentUser.DeleteSubKey("Software\\Launchy\\Dirs");
            key = Registry.CurrentUser.CreateSubKey("Software\\Launchy\\Dirs");
            key.Close();
            key = Registry.CurrentUser.OpenSubKey("Software\\Launchy\\Dirs", true);

            foreach (launchyDir d in dirs)
            {
                string entry = "";
                foreach (string type in d.types)
                {
                    entry += type;
                    entry += "%";
                }
                key.SetValue(d.path, entry);
            }
            key.Close();

        }
        private void Options_Load(object sender, EventArgs e)
        {
        
        }

        private void butAdd_Click(object sender, EventArgs e)
        {
            launchyDir d = new launchyDir();
            d.types = new List<String>();
            d.path = textPath.Text;

            string[] split = textExt.Text.Split(",".ToCharArray());

            foreach (string s in split)
            {
                d.types.Add(s);
            }
            listDirs.Items.Add(d);
            dirs.Add(d);
        }

        private void butRemove_Click(object sender, EventArgs e)
        {
            launchyDir d = (launchyDir) listDirs.SelectedItem;
            listDirs.Items.Remove(d);
            dirs.Remove(d);
        }

        private void butBrowse_Click(object sender, EventArgs e)
        {
            browseDialog.ShowDialog();
            textPath.Text = browseDialog.SelectedPath;
        }

        private void butOkay_Click(object sender, EventArgs e)
        {
            
        }


    }
}