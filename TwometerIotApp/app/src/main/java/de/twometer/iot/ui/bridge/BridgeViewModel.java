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
        new DataFetcher().execute();
    }

    LiveData<DebugInfo> getBridgeData() {
        return bridgeData;
    }

    private class DataFetcher extends AsyncTask<Void, Void, DebugInfo> {

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