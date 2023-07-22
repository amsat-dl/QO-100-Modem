using System;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace oscardata
{
    public partial class RigControlControl : UserControl
    {
        HamlibService hamlibService = new HamlibService();

        public RigControlControl()
        {
            InitializeComponent();
            frequencyLabel.Text = "";
        }

        private System.Threading.Timer hamlibTimer;

        private async void RigControlControl_Load(object sender, EventArgs e)
        {
            if (!SetupHamlib())
            {
                return;
            }

            await PopulateRigs();
        }

        private async Task PopulateRigs()
        {
            var rigs = await hamlibService.GetSupportedRigs();
            combo_radio.Items.AddRange((rigs).OrderBy(r => r.Mfg).ThenBy(r => r.Model).ToArray());
            combo_radio.SelectedItem = rigs.SingleOrDefault(s => s.Number == hamlibService.HamlibRigNumber);
            tbRigDevice.Text = hamlibService.RigDevice;
        }

        private bool SetupHamlib()
        {
            var installed = hamlibService.IsInstalled();

            Invoke(new MethodInvoker(() => tb_hamlibDetected.Text = installed ? $"{hamlibService.HamlibVersion} ({hamlibService.HamlibBitness})" : "not found"));

            if (!installed)
            {
                Invoke(new MethodInvoker(() => tb_hamlibDetected.BackColor = Color.LightPink));
                hamlibTimer = new System.Threading.Timer(_ => SetupHamlib(), null, 5000, 0);
                return false;
            }
            else
            {
                Invoke(new MethodInvoker(() => tb_hamlibDetected.BackColor = Color.LightGreen));
                Invoke(new MethodInvoker(async () => await PopulateRigs()));
                if (hamlibTimer != null)
                {
                    hamlibTimer.Change(Timeout.Infinite, Timeout.Infinite);
                    hamlibTimer.Dispose();
                }
            }

            return true;
        }

        private void btnTestPtt_Click(object sender, EventArgs e)
        {
            SetHamlibOptions();
            hamlibService.AssertPtt();
        }

        private void btnReleasePtt_Click(object sender, EventArgs e)
        {
            SetHamlibOptions();
            hamlibService.ReleasePtt();
        }

        private void SetHamlibOptions()
        {
            if (combo_radio.SelectedItem is HamlibRig rig && !string.IsNullOrWhiteSpace(tbRigDevice.Text))
            {
                hamlibService.SetRig(rig.Number, tbRigDevice.Text);
            }
            else
            {
                hamlibService.SetRig(0, null);
            }
        }

        private async void btnGetFrequency_Click(object sender, EventArgs e)
        {
            SetHamlibOptions();

            var f = await hamlibService.GetFrequency();

            frequencyLabel.Text = f == default ? "" : (f / 1000000.0).ToString("0.00000") + "MHz";
        }

        private void label4_DoubleClick(object sender, EventArgs e)
        {
            tbRigDevice.Text = "127.0.0.1:60002";
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            SetHamlibOptions();
            hamlibService.Save();
        }

        private void hamlibLinkLabel_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start("https://hamlib.github.io/");
        }
    }
}