package com.iramml.uberclone.riderapp.fragment;

import android.os.Bundle;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import com.google.android.material.bottomsheet.BottomSheetDialogFragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.iramml.uberclone.riderapp.activity.HomeActivity;
import com.iramml.uberclone.riderapp.common.Common;
import com.iramml.uberclone.riderapp.R;
import com.iramml.uberclone.riderapp.common.ConfigApp;
import com.iramml.uberclone.riderapp.retrofit.IGoogleAPI;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.net.URL;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class BottomSheetRiderFragment extends BottomSheetDialogFragment {

    String mLocation, mDestination;
    boolean isTapOnMap;
    IGoogleAPI mService;
    TextView txtCalculate, txtLocation, txtDestination;
    String startAddress;
    String endAddress;
   public String finalCalculate,finalCalculate1,finalCalculate2,Calculate,Calculate1,Calculate2;

    public static BottomSheetRiderFragment newInstance(String location, String destination, boolean isTapOnMap){
        BottomSheetRiderFragment fragment=new BottomSheetRiderFragment();
        Bundle args=new Bundle();
        args.putString("location", location);
        args.putString("destination", destination);
        args.putBoolean("isTapOnMap", isTapOnMap);
        fragment.setArguments(args);
        //getPrice1(location,destination);
        return fragment;
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mLocation=getArguments().getString("location");
        mDestination=getArguments().getString("destination");
        isTapOnMap=getArguments().getBoolean("isTapOnMap");
        //getPrice1(mLocation, mDestination);
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view0=inflater.inflate(R.layout.bottom_sheet_rider, container, false);
        txtLocation=(TextView)view0.findViewById(R.id.txtLocation);
        txtDestination=(TextView)view0.findViewById(R.id.txtDestination);
        txtCalculate=(TextView)view0.findViewById(R.id.txtCalculate);

        mService=Common.getGoogleService();
        getPrice(mLocation, mDestination);

        if(!isTapOnMap){
            //from place fragment
            txtLocation.setText(mLocation);
            txtDestination.setText(mDestination);
        }

        return view0;
    }

    private void getPrice(String mLocation, String mDestination) {
        try {
            String requestUrl = "https://maps.googleapis.com/maps/api/directions/json?mode=driving&" +
                    "transit_routing_preference=less_driving&" +
                    "origin=" + mLocation + "&" +
                    "destination=" + mDestination + "&" +
                    "key=" + ConfigApp.GOOGLE_API_KEY;
            Log.d("LINK_ROUTES", requestUrl);
            mService.getPath(requestUrl).enqueue(new Callback<String>() {
                @Override
                public void onResponse(Call<String> call, Response<String> response) {
                    try {
                        JSONObject jsonObject = new JSONObject(response.body().toString());
                        JSONArray routes = jsonObject.getJSONArray("routes");

                        JSONObject object = routes.getJSONObject(0);
                        JSONArray legs = object.getJSONArray("legs");

                        JSONObject legsObject = legs.getJSONObject(0);

                        JSONObject distance = legsObject.getJSONObject("distance");
                        String distanceText = distance.getString("text");
                        Double distanceValue = Double.parseDouble(distanceText.replaceAll("[^0-9\\\\.]", ""));

                        JSONObject time = legsObject.getJSONObject("duration");
                        String timeText = time.getString("text");
                        Integer timeValue = Integer.parseInt(timeText.replaceAll("\\D+", ""));


                       // double timevalue2 = timeValue+(0.25*timeValue);
                       // double timevalue3 = timeValue+(0.5*timeValue);
                        finalCalculate1 = String.format("%s + %s => Ksh%.0f", distanceText, timeText, Common.getPrice1(distanceValue, timeValue));
                        finalCalculate2 = String.format("%s + %s => Ksh%.0f", distanceText, timeText, Common.getPrice2(distanceValue, timeValue));
                        finalCalculate = String.format("%s + %s => Ksh%.0f", distanceText, timeText, Common.getPrice(distanceValue, timeValue));
                        txtCalculate.setText(finalCalculate);
                        Calculate=String.format("%s ( %s )", timeText, distanceText);
                        Calculate1=String.format("%s ( %s )", timeText, distanceText);
                        Calculate2=String.format("%s ( %s )", timeText, distanceText);

                        if(isTapOnMap) {
                            startAddress = legsObject.getString("start_address");
                            endAddress = legsObject.getString("end_address");

                            txtLocation.setText(startAddress);
                            txtDestination.setText(endAddress);
                            HomeActivity.car.setText(finalCalculate2);
                            HomeActivity.motorbike.setText(finalCalculate);
                            HomeActivity.tuk.setText(finalCalculate1);
                        }
                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                }

                @Override
                public void onFailure(Call<String> call, Throwable t) {
                    Log.d("ERROR", t.getMessage());
                }
            });
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    private void getPrice4(String mLocation, String mDestination) {
        try {
            String requestUrl = "https://maps.googleapis.com/maps/api/directions/json?mode=driving&" +
                    "transit_routing_preference=less_driving&" +
                    "origin=" + mLocation + "&" +
                    "destination=" + mDestination + "&" +
                    "key=" + ConfigApp.GOOGLE_API_KEY;
            Log.d("LINK_ROUTES", requestUrl);
            mService.getPath(requestUrl).enqueue(new Callback<String>() {
                @Override
                public void onResponse(Call<String> call, Response<String> response) {
                    try {
                        JSONObject jsonObject = new JSONObject(response.body().toString());
                        JSONArray routes = jsonObject.getJSONArray("routes");

                        JSONObject object = routes.getJSONObject(0);
                        JSONArray legs = object.getJSONArray("legs");

                        JSONObject legsObject = legs.getJSONObject(0);

                        JSONObject distance = legsObject.getJSONObject("distance");
                        String distanceText = distance.getString("text");
                        Double distanceValue = Double.parseDouble(distanceText.replaceAll("[^0-9\\\\.]", ""));

                        JSONObject time = legsObject.getJSONObject("duration");
                        String timeText = time.getString("text");
                        Integer timeValue = Integer.parseInt(timeText.replaceAll("\\D+", ""));
                        double timevalue2 = timeValue+(0.25*timeValue);
                        double timevalue3 = timeValue+(0.5*timeValue);
                        finalCalculate = String.format("%s + %s = Ksh%.2f", distanceText, timeText, Common.getPrice(distanceValue, timeValue));
                        finalCalculate1 = String.format("%s + %s = Ksh%.2f", distanceText, timeText, Common.getPrice(distanceValue, (int) timevalue2));
                        finalCalculate2 = String.format("%s + %s = Ksh%.2f", distanceText, timeText, Common.getPrice(distanceValue, (int) timevalue3));
                        txtCalculate.setText(finalCalculate);

                        if(isTapOnMap) {
                            startAddress = legsObject.getString("start_address");
                            endAddress = legsObject.getString("end_address");

                            txtLocation.setText(startAddress);
                            txtDestination.setText(endAddress);
                            HomeActivity.car.setText(finalCalculate);
                            HomeActivity.motorbike.setText(finalCalculate);
                            HomeActivity.tuk.setText(finalCalculate);
                        }
                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                }

                @Override
                public void onFailure(Call<String> call, Throwable t) {
                    Log.d("ERROR", t.getMessage());
                }
            });
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    public String getPrice2(String mLocation, String mDestination) {
        try {
            String requestUrl = "https://maps.googleapis.com/maps/api/directions/json?mode=driving&" +
                    "transit_routing_preference=less_driving&" +
                    "origin=" + mLocation + "&" +
                    "destination=" + mDestination + "&" +
                    "key=" + ConfigApp.GOOGLE_API_KEY;
            Log.d("LINK_ROUTES", requestUrl);
            mService.getPath(requestUrl).enqueue(new Callback<String>() {
                @Override
                public void onResponse(Call<String> call, Response<String> response) {
                    try {
                        JSONObject jsonObject = new JSONObject(response.body().toString());
                        JSONArray routes = jsonObject.getJSONArray("routes");

                        JSONObject object = routes.getJSONObject(0);
                        JSONArray legs = object.getJSONArray("legs");

                        JSONObject legsObject = legs.getJSONObject(0);

                        JSONObject distance = legsObject.getJSONObject("distance");
                        String distanceText = distance.getString("text");
                        Double distanceValue = Double.parseDouble(distanceText.replaceAll("[^0-9\\\\.]", ""));

                        JSONObject time = legsObject.getJSONObject("duration");
                        String timeText = time.getString("text");
                        Integer timeValue = Integer.parseInt(timeText.replaceAll("\\D+", ""));

                        finalCalculate = String.format("%s + %s = $%.2f", distanceText, timeText, Common.getPrice(distanceValue, timeValue));
                        txtCalculate.setText(finalCalculate);

                        if(isTapOnMap) {
                            //startAddress = legsObject.getString("start_address");
                            //endAddress = legsObject.getString("end_address");

                            //txtLocation.setText(startAddress);
                           // txtDestination.setText(endAddress);

                        }
                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                }

                @Override
                public void onFailure(Call<String> call, Throwable t) {
                    Log.d("ERROR", t.getMessage());
                }
            });
        } catch (Exception ex) {
            ex.printStackTrace();
        }
        return  finalCalculate;
    }



    public String sa(){
        return startAddress;
    }

    public String da(){
        return endAddress;
    }


    public String pr(){
        return  finalCalculate;
    }
}
