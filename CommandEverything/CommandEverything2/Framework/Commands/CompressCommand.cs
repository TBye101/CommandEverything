using CommandEverything.Framework.Util;
using CommandEverything.Framework.Util.Text;
using ICSharpCode.SharpZipLib.Core;
using ICSharpCode.SharpZipLib.Zip;
using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CommandEverything.Framework.WIP
{
    public class CompressCommand : ICommand
    {
        public string GetHelp()
        {
            return "Used to compress a file.";
        }

        public string GetName()
        {
            return "Compress File";
        }

        public void Run(string Input)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.FileName = "File";
            dlg.DefaultExt = ".txt";
            dlg.Multiselect = true;

            // Show open file dialog box
            DialogResult result = dlg.ShowDialog();

            SaveFileDialog sv = new SaveFileDialog();
            sv.FileName = dlg.FileName;
            sv.DefaultExt = ".ce";

            DialogResult res = sv.ShowDialog();

            // Process open file dialog box results
            if (result == DialogResult.OK && res == DialogResult.OK)
            {
                ZipUtils.ZipFiles(dlg.FileNames.ToList(), sv.FileName, 9);

                ConsoleWriter.WriteLine("Compressed file");
            }
            else
            {
                ConsoleWriter.WriteLine("Error!");
            }
        }

        public bool ShouldRunThisCommand(string Input)
        {
            string[] Valid = { "compress", "compress file", "shrink file"};
            return Utility.DoesStringContain(Input, Valid);
        }
    }
}