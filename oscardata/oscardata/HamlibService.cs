using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using static oscardata.Command;

namespace oscardata
{
    internal class HamlibService
    {
        public Version HamlibVersion { get; private set; }
        public string HamlibBitness { get; private set; }
        private string rigctlFile;
        private string appdataPath;
        private string settingsFile => Path.Combine(appdataPath, "audiointerface.json");

        public HamlibService()
        {
            var hamlib = Task.Run(FindHamlib).Result;

            if (hamlib != default)
            {
                rigctlFile = Path.Combine(hamlib.Item1, "bin", "rigctl.exe");
                HamlibVersion = hamlib.Item2;
                HamlibBitness = hamlib.Item3;
            }

            var path = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
            appdataPath = Path.Combine(path, "hamradioapps");
            if (!Directory.Exists(appdataPath))
            {
                Directory.CreateDirectory(appdataPath);
            }

            if (File.Exists(settingsFile))
            {
                var settings = JsonConvert.DeserializeObject<HamlibSettings>(File.ReadAllText(settingsFile));
                HamlibRigNumber = settings.RigNumber;
                RigDevice = settings.Device;
            }
        }

        private static async Task<(string folder, Version version, string bitness)> FindHamlib()
        {
            var hamlibFolders = new List<(string, Version, string)>();
            foreach (var pf in new[] { Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles), Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86) })
            {
                if (!Directory.Exists(pf)) continue;

                foreach (var dir in Directory.GetDirectories(pf, "hamlib*"))
                {
                    var rigCtlPath = Path.Combine(dir, "bin", "rigctl.exe");
                    if (!File.Exists(rigCtlPath)) continue;
                    var (stdout, _) = await ReadAsync(rigCtlPath, "--version");
                    var parts = stdout.Split(new[] { ' ', '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);

                    foreach (var part in parts)
                    {
                        if (Version.TryParse(part.Trim(), out var version))
                        {
                            hamlibFolders.Add((dir, version,
                                dir.Contains("x86") ? "32 bit" : "64 bit"));
                            break;
                        }
                    }
                }
            }

            if (!hamlibFolders.Any()) return default;

            return hamlibFolders.OrderByDescending(hl => hl.Item2).First();
        }

        internal bool IsInstalled()
        {
            return File.Exists(rigctlFile);
        }

        public async Task<ICollection<HamlibRig>> GetSupportedRigs()
        {
            var (stdout, _) = await ReadAsync(rigctlFile, "-l");

            var rigs = new List<HamlibRig>();

            var lines = stdout.Split('\r', '\n');

            var header = lines.First();

            var startOfMfgCol = header.IndexOf("Mfg");
            var startOfModelCol = header.IndexOf("Model");
            var startOfVersionCol = header.IndexOf("Version");
            var startOfStatusCol = header.IndexOf("Status");

            var mfgColWidth = startOfModelCol - startOfMfgCol;
            var modelColWidth = startOfVersionCol - startOfModelCol;
            var versionColWidth = startOfStatusCol - startOfVersionCol;

            var data = lines.Skip(1);

            foreach (var line in data)
            {
                if (string.IsNullOrWhiteSpace(line)) continue;
                var number = line.Substring(0, 6);
                var mfg = line.Substring(startOfMfgCol, mfgColWidth);
                var model = line.Substring(startOfModelCol, modelColWidth);
                var version = line.Substring(startOfVersionCol, versionColWidth);

                var rig = new HamlibRig
                {
                    Number = int.Parse(number),
                    Mfg = mfg.Trim(),
                    Model = model.Trim(),
                    Version = version.Trim(),
                };

                rigs.Add(rig);
            }

            return rigs;
        }

        public int HamlibRigNumber { get; private set; }
        public string RigDevice { get; private set; }

        public void SetRig(int hamlibNumber, string rigDevice)
        {
            HamlibRigNumber = hamlibNumber;
            RigDevice = rigDevice;
        }

        public void Save()
        {
            var json = JsonConvert.SerializeObject(new HamlibSettings { Device = RigDevice, RigNumber = HamlibRigNumber }, Formatting.Indented);
            File.WriteAllText(settingsFile, json);
        }

        private class HamlibSettings
        {
            public int RigNumber { get; set; }
            public string Device { get; set; }
        }

        internal async Task<long> GetFrequency()
        {
            if (rigctlFile == null) return default;
            var (stdOut, _) = await ReadAsync(rigctlFile, $"{GetArgs()} f");
            if (string.IsNullOrWhiteSpace(stdOut))
            {
                return default;
            }
            return long.Parse(stdOut.Trim());
        }

        private string GetArgs()
        {
            return $"-m {HamlibRigNumber} -r {RigDevice}";
        }

        internal async void AssertPtt()
        {
            if (rigctlFile == null) return;
            await ReadAsync(rigctlFile, $"{GetArgs()} T 1");
        }

        internal async void ReleasePtt()
        {
            if (rigctlFile == null) return;
            await ReadAsync(rigctlFile, $"{GetArgs()} T 0");
        }
    }

    internal class HamlibRig
    {
        public int Number { get; set; }
        public string Mfg { get; set; }
        public string Model { get; set; }
        public string Version { get; set; }

        public override string ToString() => $"{Mfg} {Model}";
    }
}
