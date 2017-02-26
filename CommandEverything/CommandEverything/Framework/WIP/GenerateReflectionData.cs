using CommandEverything.Framework.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CommandEverything.Framework.WIP
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
            Assembly o;
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
                //ConsoleWriter.WriteLine("File invalid!");
            }
        }

        public bool ShouldRunThisCommand(string Input)
        {
            string[] Valid = { "generate reflection data", "generate data", "create reflection data", "get reflection data" };
            return Utility.DoesStringContain(Input, Valid);
        }
    }
}
