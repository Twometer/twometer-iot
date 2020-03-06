package de.twometer.iot.ui.bridge;

import android.os.AsyncTask;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import de.twometer.iot.IoTContext;
import de.twometer.iot.model.DebugInfo;

public class BridgeViewModel extends ViewModel {

    private MutableLiveData<DebugInfo> bridgeData;

    public BridgeViewModel() {
        bridgeData = new MutableLiveData<>();
        refresh();
    }

    LiveData<DebugInfo> getBridgeData() {
        return bridgeData;
    }

    void refresh() {
        new DataFetcher(bridgeData).execute();
    }

    private static class DataFetcher extends AsyncTask<Void, Void, DebugInfo> {

        private MutableLiveData<DebugInfo> bridgeData;

        DataFetcher(MutableLiveData<DebugInfo> bridgeData) {
            this.bridgeData = bridgeData;
        }

        @Override
        protected DebugInfo doInBackground(Void... voids) {
            return IoTContext.getInstance().getClient().getDebugInfo();
        }

        @Override
        protected void onPostExecute(DebugInfo debugInfo) {
            bridgeData.setValue(debugInfo);
        }
    }

}