using System.Diagnostics;
using System.Threading.Tasks;

namespace oscardata
{
    internal static class Command
    {
        public static async Task<(string stdout, string stderr)> ReadAsync(string program, string args)
        {
            using (var process = new Process())
            {
                process.StartInfo.FileName = program;
                process.StartInfo.Arguments = args;
                process.StartInfo.CreateNoWindow = true;
                process.StartInfo.UseShellExecute = false;
                process.StartInfo.RedirectStandardOutput = process.StartInfo.RedirectStandardError = true;
                process.Start();
                var stdout = await process.StandardOutput.ReadToEndAsync();
                var stderr = await process.StandardError.ReadToEndAsync();
                return (stdout, stderr);
            }
        }
    }
}