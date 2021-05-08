using CSCore;
using CSCore.DSP;
using CSCore.SoundIn;
using CSCore.Streams;
using LumiSync.Audio;
using LumiSync.Net;
using LumiSync.SoundLink;
using MahApps.Metro.Controls;
using MahApps.Metro.Controls.Dialogs;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace LumiSync
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : MetroWindow
    {
        private Bridge bridge;

        private List<ISoundLinkStrategy> strategies = new List<ISoundLinkStrategy>()
        {
            new ColorCycleStrategy()
        };

        public MainWindow()
        {
            InitializeComponent();
            Initialize();

            SyncTypeBox.ItemsSource = strategies;
            SyncTypeBox.SelectedIndex = 0;
        }

        private async void Initialize()
        {
            var controller = await this.ShowProgressAsync("Connecting...", "Please wait while LumiSync is contacting the IoT bridge...");
            controller.SetIndeterminate();
            controller.SetCancelable(true);

            controller.Canceled += (s, e) =>
            {
                controller.CloseAsync();
            };

            bridge = await BridgeDiscovery.Discover();
            await controller.CloseAsync();

            StatusLabel.Content = $"Connected to bridge at {bridge.IpAddress}";
            RefreshData();
        }

        private async void RefreshData()
        {
            var devices = await bridge.GetDevices();
            DeviceListView.ItemsSource = devices;
            MusicSyncDeviceSelectorListView.ItemsSource = devices;
        }

        private void RefreshButton_Click(object sender, RoutedEventArgs e)
        {
            RefreshData();
        }

        private WasapiCapture capture;
        private SoundInSource source;
        private SpectrumAnalyzer spectrumAnalyzer;
        private bool syncActive = false;
        private Color lastColor;

        private void SoundSyncToggleButton_Click(object sender, RoutedEventArgs e)
        {
            if (capture == null)
            {
                capture = new WasapiLoopbackCapture();
                capture.Initialize();
                capture.Start();

                source = new SoundInSource(capture);
                spectrumAnalyzer = new SpectrumAnalyzer(source.ToSampleSource());

                source.DataAvailable += (_, args) =>
                {
                    if (!syncActive)
                        return;

                    spectrumAnalyzer.Update();

                    float power = Math.Min(spectrumAnalyzer.BandFilter(4)[0], 100);

                    Dispatcher.Invoke(() =>
                    {
                        var strat = (ISoundLinkStrategy)SyncTypeBox.SelectedItem;
                        var color = strat.Update(spectrumAnalyzer);
                        var wpfcolor = Color.FromRgb(color.R, color.G, color.B);
                        PowerMeterBar.Value = power;
                        PowerMeterBar.Foreground = new SolidColorBrush(wpfcolor);

                        if (wpfcolor != lastColor)
                        {
                            foreach (var itm in MusicSyncDeviceSelectorListView.SelectedItems)
                            {
                                if (itm is Device device && device.Type == "LIGHT")
                                {
                                    bridge.SetColor(device.DeviceId, "Light.ColorStream", color);
                                }
                            }
                        }
                        lastColor = wpfcolor;
                    });
                };

                Console.WriteLine("Opened capture device");
            }

            syncActive = !syncActive;

            if (syncActive)
                SyncStatusText.Text = "Sync enabled";
            else
                SyncStatusText.Text = "Sync disabled";
        }


    }
}
