using CommandEverything.Framework.Util.Text;
using Octokit;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Windows.Forms;

namespace CommandEverything.Framework.Util
{
    /// <summary>
    /// Automatically reports exceptions to my Github page.
    /// </summary>
    public static class Error
    {
        /// <summary>
        /// Used by this to tell if we should fail a test because this is called.
        /// </summary>
        public static bool IsUnitTesting = false;

        /// <summary>
        /// The name of the Git user to report errors as.
        /// </summary>
        private static string UserName = "SneakyTactician";

        /// <summary>
        /// The repository to report to.
        /// </summary>
        private static string RepositoryName = "CommandEverything";

        private static string GitHubRepoToken = Secure.Token;

        /// <summary>
        /// Used to interact with Github.
        /// </summary>
        private static GitHubClient Client = ConstructClient();

        /// <summary>
        /// Auto reports an error to my Github issues section.
        /// </summary>
        /// <param name="Ex"></param>
        public static void Report(Exception Ex)
        {
            try
            {
                if (IsUnitTesting)
                {
                    throw Ex;
                }

                string Report = GenerateReport(Ex);

                string[] Write = { "An Error has occured", Report };
                ConsoleWriter.WriteAll(Write, "Error!");

                string IssueTitle = "Error in method: " + Ex.TargetSite.Name + ", error code: " + Ex.HResult + " Assembly Version: " + Assembly.GetExecutingAssembly().GetName().Version.ToString();

                NewIssue ToReport = new NewIssue(IssueTitle) { Body = Report };
                ToReport.Labels.Add("bug");

                var AllIssues = Client.Issue.GetAllForRepository(UserName, RepositoryName).Result;

                List<Issue> Issues = AllIssues.ToList<Issue>();

                int I = 0;
                int Siz = AllIssues.Count;

                while (I != Siz)
                {
                    if (Issues.ElementAt(I).Title == ToReport.Title)
                    {
                        return;
                    }

                    I++;
                }
                var Observable = Client.Issue.Create(UserName, RepositoryName, ToReport);
            }
            #pragma warning disable CS0168 // Variable is declared but never used
            catch (Exception E)
            #pragma warning restore CS0168 // Variable is declared but never used
            {
                if (IsUnitTesting)
                {
                    throw Ex;
                }
                else
                {
                    //Eat it
                }
            }
        }

        /// <summary>
        /// Constructs and authenticates the github client.
        /// </summary>
        /// <returns></returns>
        private static GitHubClient ConstructClient()
        {
            try
            {
                GitHubClient Ret = new GitHubClient(new Octokit.ProductHeaderValue(UserName));
                Credentials Creds = new Credentials(UserName, GitHubRepoToken);
                Ret.Credentials = Creds;
                return Ret;
            }
            #pragma warning disable CS0168 // Variable is declared but never used
            catch (Exception E)
            #pragma warning restore CS0168 // Variable is declared but never used
            {
                //Eat it
                Debug.Assert(false);
                return null;
            }
        }

        /// <summary>
        /// Generates a report to post to Github's issue tracker for this game.
        /// </summary>
        /// <param name="Ex"></param>
        /// <returns></returns>
        private static string GenerateReport(Exception Ex)
        {
            try
            {
                List<string> Report = new List<string>();

                #region Exception

                Report.Add("**Exception**");
                Report.Add("Error code: " + Ex.HResult);
                Report.Add("Error message: " + Ex.Message);
                Report.Add("Method: " + Ex.TargetSite.Name);
                Report.Add("Source: " + Ex.Source);
                Report.Add("\r\n");
                Report.Add("**Stack trace**");
                Report.Add(Ex.StackTrace);
                Report.Add("\r\n");
                Report.Add("Type: " + Ex.GetType());

                #endregion Exception

                #region InnerExceptions

                if (Ex.InnerException != null)
                {
                    Report.Add("\r\n");
                    Report.Add("**Inner Exceptions**" + GenerateReportForException(Ex.InnerException, 1));
                    Report.Add("\r\n");
                }

                #endregion InnerExceptions

                #region OtherInformation

                Report.Add("System information:");
                Report.Add("Processor count: " + Environment.ProcessorCount.ToString());

                Process Current = Process.GetCurrentProcess();

                Report.Add("\r\n");
                Report.Add("Application information: ");
                Report.Add("Non paged system memory: " + Current.NonpagedSystemMemorySize64.ToString());
                Report.Add("Paged memory: " + Current.PagedMemorySize64.ToString());
                Report.Add("Max memory usage: " + Current.PeakWorkingSet64.ToString());
                Report.Add("Threads running: " + Current.Threads.Count.ToString());

                //Report.Add(GetReflectionData(ex));

                #endregion OtherInformation

                return ConvertListToString(Report);
            }
            #pragma warning disable CS0168 // Variable is declared but never used
            catch (Exception E)
            #pragma warning restore CS0168 // Variable is declared but never used
            {
                //Eat it
                return "**Report Generation Failed**";
            }
        }

        /// <summary>
        /// Generates data about the inner exception in the exception.
        /// </summary>
        /// <param name="Ex"></param>
        /// <param name="InnerException"></param>
        /// <returns></returns>
        public static string GenerateReportForException(Exception Ex, int InnerException)
        {
            try
            {
                List<string> Report = new List<string>();

                int ExceptionCount = InnerException;

                #region CollectData

                Report.Add("\r\n");
                Report.Add("Inner Exception # " + InnerException.ToString() + ":");
                Report.Add("Error code: " + Ex.HResult);

                if (Ex.Message != null)
                {
                    Report.Add("Error message: " + Ex.Message);
                }

                if (Ex.HelpLink != null)
                {
                    Report.Add("Help link: " + Ex.HelpLink);
                }

                if (Ex.TargetSite != null)
                {
                    Report.Add("Method: " + Ex.TargetSite.Name);
                }

                if (Ex.Source != null)
                {
                    Report.Add("Source: " + Ex.Source);
                }

                Report.Add("\r\n");
                Report.Add("**Stack trace**: ");

                if (Ex.StackTrace != null)
                {
                    Report.Add(Ex.StackTrace);
                }

                Report.Add("\r\n");

                if (Ex.GetType() != null)
                {
                    Report.Add("Type: " + Ex.GetType());
                }

                ExceptionCount++;

                if (Ex.InnerException != null)
                {
                    Report.Add(GenerateReportForException(Ex, ExceptionCount));
                }

                #endregion CollectData

                return ConvertListToString(Report);
            }
            #pragma warning disable CS0168 // Variable is declared but never used
            catch (Exception TheException)
            #pragma warning restore CS0168 // Variable is declared but never used
            {
                Debug.Assert(false);
                return "**Error while generating inner exception data**";
            }
        }

        /// <summary>
        /// Converts a list full of strings into one null terminated string.
        /// </summary>
        /// <param name="List"></param>
        /// <returns></returns>
        public static string ConvertListToString(List<string> List)
        {
            string[] Array = List.ToArray();
            string Rep = "";

            int I = 0;
            int Size = Array.Count();

            while (I != Size)
            {
                string Current = Array[I];

                if (Current != "\r\n")
                {
                    Current += "\r\n";
                }

                Rep += Current;

                I++;
            }

            return Rep;
        }
    }
}