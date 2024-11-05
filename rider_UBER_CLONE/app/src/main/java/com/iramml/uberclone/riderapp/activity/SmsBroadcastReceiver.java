package com.iramml.uberclone.riderapp.activity;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import com.google.android.gms.auth.api.phone.SmsRetriever;
import com.google.android.gms.common.api.CommonStatusCodes;
import com.google.android.gms.common.api.Status;

import java.util.Objects;

public class SmsBroadcastReceiver extends BroadcastReceiver{
    SmsBroadcastReceiverListener smsBroadcastReceiverListener;
    public SmsRetriever ikosht;
    @Override
    public void onReceive(Context context, Intent intent) {
        if (Objects.equals(intent.getAction(), ikosht.SMS_RETRIEVED_ACTION)) {
            Bundle extras = intent.getExtras();
            Status smsRetrieverStatus = (Status) Objects.requireNonNull(extras).get(ikosht.EXTRA_STATUS);
            switch (Objects.requireNonNull(smsRetrieverStatus).getStatusCode()) {
                case CommonStatusCodes.SUCCESS:
                    Intent messageIntent = extras.getParcelable(ikosht.EXTRA_CONSENT_INTENT);
                    if (!(smsBroadcastReceiverListener ==null)){
                        smsBroadcastReceiverListener.onSuccess(messageIntent);}
                    break;
                case CommonStatusCodes.TIMEOUT:
                    if (!(smsBroadcastReceiverListener ==null)){
                    smsBroadcastReceiverListener.onFailure();}
                    break;
            }
        }
    }
    public interface SmsBroadcastReceiverListener {
        void onSuccess(Intent intent);
        void onFailure();
        //TIMEOUT
    }
}