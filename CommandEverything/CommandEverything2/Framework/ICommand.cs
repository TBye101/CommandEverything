namespace CommandEverything.Framework
{
    /// <summary>
    /// Every command should implement this.
    /// </summary>
    public interface ICommand
    {
        /// <summary>
        /// This function returns a value indicating if the specified input means this command should be run.
        /// </summary>
        /// <param name="Input"></param>
        /// <returns></returns>
        bool ShouldRunThisCommand(string Input);

        /// <summary>
        /// Tells this command to run.
        /// </summary>
        /// <param name="Input"></param>
        void Run(string Input);

        /// <summary>
        /// Returns the name of this command.
        /// </summary>
        /// <returns></returns>
        string GetName();

        /// <summary>
        /// Returns some help information about this command.
        /// </summary>
        /// <returns></returns>
        string GetHelp();
    }
}