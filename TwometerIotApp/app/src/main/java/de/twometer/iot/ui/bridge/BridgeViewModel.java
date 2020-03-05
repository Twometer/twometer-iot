package de.twometer.iot.ui.bridge;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class BridgeViewModel extends ViewModel {

    private MutableLiveData<String> mText;

    public BridgeViewModel() {
        mText = new MutableLiveData<>();
        mText.setValue("This is bridge fragment");
    }

    LiveData<String> getText() {
        return mText;
    }
}