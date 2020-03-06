package de.twometer.iot.ui.devices;

import android.os.AsyncTask;

import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import java.util.List;

import de.twometer.iot.IoTContext;
import de.twometer.iot.model.Device;

public class DevicesViewModel extends ViewModel {

    private MutableLiveData<List<Device>> devices;

    public DevicesViewModel() {
        devices = new MutableLiveData<>();
        refresh();
    }

    void refresh() {
        new DataFetcher(devices).execute();
    }

    MutableLiveData<List<Device>> getDevices() {
        return devices;
    }

    private static class DataFetcher extends AsyncTask<Void, Void, List<Device>> {

        private MutableLiveData<List<Device>> devices;

        DataFetcher(MutableLiveData<List<Device>> devices) {
            this.devices = devices;
        }

        @Override
        protected List<Device> doInBackground(Void... voids) {
            return IoTContext.getInstance().getClient().getDevices();
        }

        @Override
        protected void onPostExecute(List<Device> devices) {
            this.devices.setValue(devices);
        }
    }
}