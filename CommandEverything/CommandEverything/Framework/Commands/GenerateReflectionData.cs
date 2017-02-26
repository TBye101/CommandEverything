using CommandEverything.Framework.Util;
using CommandEverything.Framework.Util.Text;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CommandEverything.Framework.Commands
{
    /// <summary>
    /// Generates as much data as it can about the specified dll/exe.
    /// </summary>
    public class GenerateReflectionData : ICommand
    {
        public string GetHelp()
        {
            return "Generates reflection data about a chosen assembly and saves that data to a file.";
        }

        public string GetName()
        {
            return "Generate Reflection Data";
        }

        public void Run(string Input)
        {
            Assembly o = null;
            OpenFileDialog dlg = new OpenFileDialog();

            dlg.FileName = "Library"; // Default file name
            dlg.DefaultExt = ".dll"; // Default file extension

            // Show open file dialog box
            DialogResult result = dlg.ShowDialog();

            // Process open file dialog box results
            if (result == DialogResult.OK)
            {
                // Open document
                //string[] script = File.ReadAllLines(dlg.FileName, Encoding.Default);
                //this.AttemptToExecuteAll(script);
                o = Assembly.LoadFile(dlg.FileName);
            }
            else
            {
                ConsoleWriter.WriteLine("File invalid!");
            }

            SaveFileDialog Sv = new SaveFileDialog();
            Sv.AddExtension = true;
            Sv.CheckPathExists = true;
            Sv.DefaultExt = ".txt";

            if (Sv.ShowDialog() == DialogResult.OK)
            {
                Stream s = File.Open(Sv.FileName, FileMode.CreateNew);
                StreamWriter sw = new StreamWriter(s);
                Reflection a;
                a = new Reflection();
                a.GenerateData(sw, o);

                s.Flush();
                s.Close();
                sw.Flush();
                sw.Close();
            }
        }

        public bool ShouldRunThisCommand(string Input)
        {
            string[] Valid = { "generate reflection data", "generate data", "create reflection data", "get reflection data" };
            return Utility.DoesStringContain(Input, Valid);
        }
    }
}
