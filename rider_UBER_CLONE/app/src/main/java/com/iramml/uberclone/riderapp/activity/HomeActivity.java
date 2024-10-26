package com.iramml.uberclone.riderapp.activity;

import android.Manifest;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentSender;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.location.Geocoder;
import android.location.LocationManager;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AlertDialog;
import androidx.core.app.ActivityCompat;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.os.Handler;
import android.provider.Settings;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.util.Log;
import android.util.TypedValue;
import android.view.LayoutInflater;
import android.view.View;

import com.google.android.gms.common.api.PendingResult;
import com.google.android.gms.location.FusedLocationProviderApi;
import com.google.android.gms.location.LocationListener;
import com.google.android.gms.location.LocationRequest;
import com.google.android.gms.location.LocationServices;
import com.google.android.gms.location.LocationSettingsRequest;
import com.google.android.gms.location.LocationSettingsResult;
import com.google.android.gms.location.LocationSettingsStatusCodes;
import com.google.android.gms.location.places.Places;
import com.google.android.gms.maps.CameraUpdate;
import com.google.android.gms.maps.MapFragment;
import com.google.android.gms.maps.MapView;
import com.google.android.gms.maps.model.Polyline;
import com.google.android.gms.maps.model.PolylineOptions;
import com.google.android.material.floatingactionbutton.FloatingActionButton;
import com.google.android.material.navigation.NavigationView;

import androidx.core.view.GravityCompat;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;

import android.view.MenuItem;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.messaging.FirebaseMessaging;
import com.google.gson.Gson;
import com.iramml.uberclone.riderapp.adapter.ClickListener;
import com.iramml.uberclone.riderapp.common.Common;
import com.iramml.uberclone.riderapp.common.ConfigApp;
import com.iramml.uberclone.riderapp.fragment.BottomSheetRiderFragment;
import com.iramml.uberclone.riderapp.helper.CustomInfoWindow;
import com.iramml.uberclone.riderapp.helper.DirectionJSONParser;
import com.iramml.uberclone.riderapp.interfaces.HttpResponse;
import com.iramml.uberclone.riderapp.interfaces.IFCMService;
import com.iramml.uberclone.riderapp.interfaces.googleAPIInterface;
import com.iramml.uberclone.riderapp.interfaces.locationListener;
import com.iramml.uberclone.riderapp.messages.Errors;
import com.iramml.uberclone.riderapp.messages.ShowMessage;
import com.iramml.uberclone.riderapp.model.firebase.User;
import com.iramml.uberclone.riderapp.model.placesapi.PlacesResponse;
import com.iramml.uberclone.riderapp.model.placesapi.Results;
import com.iramml.uberclone.riderapp.R;
import com.iramml.uberclone.riderapp.retrofit.RetrofitClient;
import com.iramml.uberclone.riderapp.util.Location;
import com.facebook.AccessToken;
import com.facebook.GraphRequest;
import com.facebook.GraphResponse;
import com.facebook.login.LoginManager;
import com.firebase.geofire.GeoFire;
import com.firebase.geofire.GeoLocation;
import com.firebase.geofire.GeoQuery;
import com.firebase.geofire.GeoQueryEventListener;
import com.google.android.gms.auth.api.Auth;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.auth.api.signin.GoogleSignInOptions;
import com.google.android.gms.auth.api.signin.GoogleSignInResult;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.api.OptionalPendingResult;
import com.google.android.gms.common.api.ResultCallback;
import com.google.android.gms.common.api.Status;
import com.google.android.gms.location.LocationResult;
import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.BitmapDescriptorFactory;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MapStyleOptions;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.OnProgressListener;
import com.google.firebase.storage.StorageReference;
import com.google.firebase.storage.UploadTask;
import com.iramml.uberclone.riderapp.util.NetworkUtil;
import com.iramml.uberclone.riderapp.adapter.RecyclerViewPlaces.PlacesAdapter;
import com.karumi.dexter.Dexter;
import com.karumi.dexter.MultiplePermissionsReport;
import com.karumi.dexter.PermissionToken;
import com.karumi.dexter.listener.PermissionRequest;
import com.karumi.dexter.listener.multi.MultiplePermissionsListener;
import com.rengwuxian.materialedittext.MaterialEditText;
import com.squareup.picasso.Picasso;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Objects;
import java.util.UUID;

import de.hdodenhof.circleimageview.CircleImageView;
import dmax.dialog.SpotsDialog;
import kotlinx.coroutines.Delay;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class HomeActivity extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener, OnMapReadyCallback, GoogleApiClient.ConnectionCallbacks, GoogleApiClient.OnConnectionFailedListener,
        GoogleMap.OnInfoWindowClickListener {
    private ImageView carr, motorb, tukk;
    private Button btnRequestPickup;
    private FloatingActionButton btnRequestPickup2, btnRequestPickup3,btnRequestPickup4;
    double latitude, longitude;
    private Toolbar toolbar;
    private Double lat1, lng1;
    public String avata;
    private GoogleMap mMap;
    private android.location.Location mLastLocation;
    private LinearLayout llPickupInput, llDestinationInput, llPickupPlace, llDestinationPlace;
    private EditText etFinalPickup, etFinalDestination, etPickup, etDestination;
    private RecyclerView rvPickupPlaces, rvDestinationPlaces;
    private GoogleSignInAccount account;
    public static TextView car, motorbike, tuk;
    private static final int REQUEST_CHECK_SETTINGS = 1000;
    private Marker riderMarket, destinationMarker,riderMarket2;
    private final ArrayList<Marker> driverMarkers = new ArrayList<>();
    BottomSheetRiderFragment mBottomSheet;
    private GoogleApiClient mGoogleApiClient;
    AccessToken accessToken = AccessToken.getCurrentAccessToken();
    boolean isLoggedInFacebook = accessToken != null && !accessToken.isExpired();
    String c;
    private googleAPIInterface mService;
    private DatabaseReference driversAvailable;
    private StorageReference storageReference;
    private IFCMService ifcmService;
    Marker now;
    private Location location;
    private NetworkUtil networkUtil;
    Geocoder geocoder;
    List<android.location.Address> addresses;
    private String mPlaceLocation, mPlaceDestination;
    private Double currentLat, currentLng;
    private boolean ca = true, moto = false, tu = false, pickupPlacesSelected = false, place1 = false;
    private double radius = 0.5, distance = 0.5; // km android:theme="@style/AppTheme"
    private static final int LIMIT = 3;
    private String URL_BASE_API_PLACES = "https://maps.googleapis.com/maps/api/place/textsearch/json?";
    private LocationRequest request;
    public static boolean driverFound = false;
    MapFragment mapFragment;
    private Polyline direction;
    LatLng latLng2=null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //hideStatusBar();
        setContentView(R.layout.activity_home);
        initViews();
        verifyGoogleAccount();
        mapFragment = (MapFragment) getFragmentManager().findFragmentById(R.id.map);
        FirebaseStorage storage = FirebaseStorage.getInstance();
        storageReference = storage.getReference();
        ifcmService = Common.getFCMService();
        networkUtil = new NetworkUtil(this);
        mService = RetrofitClient.getClient().create(googleAPIInterface.class);
        //CheckMapPermission();
        car.setText("Vika Car request pickup");
        motorbike.setText("Vika Motorbike request pickup");
        tuk.setText("Vika tuk tuk request pickup");

        location = new Location(this, new locationListener() {
            @Override
            public void locationResponse(LocationResult response) {
                currentLat = Objects.requireNonNull(response.getLastLocation()).getLatitude();
                currentLng = response.getLastLocation().getLongitude();
                Common.currenLocation = new LatLng(response.getLastLocation().getLatitude(), response.getLastLocation().getLongitude());
                displayLocation();
                if (mPlaceLocation == null) {
                    driversAvailable = FirebaseDatabase.getInstance().getReference(Common.driver_tbl);
                    driversAvailable.addValueEventListener(new ValueEventListener() {
                        @Override
                        public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                            if (!(Common.currenLocation1 == null)) {
                                loadAllAvailableDriver(new LatLng(lat1, lng1));
                            } else {
                                loadAllAvailableDriver(new LatLng(currentLat, currentLng));
                            }

                        }

                        @Override
                        public void onCancelled(@NonNull DatabaseError databaseError) {

                        }
                    });
                }
                loadcurrent();

            }
        });


        SupportMapFragment mapFragment = (SupportMapFragment) getSupportFragmentManager()
                .findFragmentById(R.id.map);
        Objects.requireNonNull(mapFragment).getMapAsync(this);

        carr = findViewById(R.id.selectedUberX);
        motorb = findViewById(R.id.selectedUberBlack);
        tukk = findViewById(R.id.selectedUberX1);

        carr.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    btnRequestPickup.setText("REQUEST");
                    driverFound = false;
                    boolean isToggle = !ca;
                    ca = true;
                    tu = false;
                    moto = false;
                    carr.setImageResource(R.drawable.car_cui3);
                    motorb.setImageResource(R.drawable.car_vip_select3);
                    tukk.setImageResource(R.drawable.tuk);
                    if (mBottomSheet.finalCalculate2 != null)
                        car.setText(mBottomSheet.finalCalculate2);
                    else
                        car.setText("Vika Car request pickup");
                    motorbike.setText("Vika Motorbike request pickup");
                    tuk.setText("Vika tuk tuk request pickup");
                    if (!(Common.currenLocation1 == null)) {
                        loadAllAvailableDriver(new LatLng(lat1, lng1));
                    } else {
                        loadAllAvailableDriver(new LatLng(currentLat, currentLng));
                    }
                } catch (Exception e) {

                }
            }
        });

        motorb.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    btnRequestPickup.setText("REQUEST");
                    driverFound = false;
                    boolean isToggle = ca;
                    moto = true;
                    ca = false;
                    tu = false;
                    carr.setImageResource(R.drawable.car_cui_select3);
                    motorb.setImageResource(R.drawable.car_vip3);
                    tukk.setImageResource(R.drawable.tuk);
                    car.setText("Vika Car request pickup");
                    // motorbike.setText("Vika Motorbike");
                    if (mBottomSheet.finalCalculate != null)
                        motorbike.setText(mBottomSheet.finalCalculate);
                    else
                        motorbike.setText("Vika Motorbike request pickup");
                    tuk.setText("Vika tuk tuk request pickup");
                    if (!(Common.currenLocation1 == null)) {
                        loadAllAvailableDriver(new LatLng(lat1, lng1));
                    } else {
                        loadAllAvailableDriver(new LatLng(currentLat, currentLng));
                    }

                } catch (Exception e) {

                }
            }
        });
        tukk.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    btnRequestPickup.setText("REQUEST");
                    driverFound = false;
                    boolean isToggle = !ca;
                    ca = false;
                    moto = false;
                    tu = true;
                    carr.setImageResource(R.drawable.car_cui_select3);
                    motorb.setImageResource(R.drawable.car_vip_select3);
                    tukk.setImageResource(R.drawable.tuk1);
                    car.setText("Vika Car request pickup");
                    motorbike.setText("Vika Motorbike request pickup");
                    if (mBottomSheet.finalCalculate1 != null)
                        tuk.setText(mBottomSheet.finalCalculate1);
                    else
                        tuk.setText("Vika tuk tuk request pickup");


                    if (!(Common.currenLocation1 == null)) {
                        loadAllAvailableDriver(new LatLng(lat1, lng1));
                    } else {
                        loadAllAvailableDriver(new LatLng(currentLat, currentLng));
                    }
                } catch (Exception e) {

                }
            }
        });

        btnRequestPickup = findViewById(R.id.btnPickupRequest);
        btnRequestPickup.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (currentLat != null && currentLng != null) {

                    place1 = false;
                    if (!driverFound) {
                        requestPickup(Common.userID);
                    } else {
                        if (!(Common.currenLocation1 == null)) {
                            Common.sendRequestToDriver(Common.driverID, ifcmService, getApplicationContext(), Common.currenLocation1);
                            Common.currenLocation1 = null;
                        } else {
                            Common.sendRequestToDriver(Common.driverID, ifcmService, getApplicationContext(), Common.currenLocation);
                        }
                    }
                }
            }
        });

        btnRequestPickup2 = (FloatingActionButton) findViewById(R.id.floatingActionButton9);
        btnRequestPickup2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Toast.makeText(getApplicationContext(), "nison", Toast.LENGTH_SHORT).show();
                DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
                drawer.openDrawer(GravityCompat.START);


            }
        });

        btnRequestPickup3 = (FloatingActionButton) findViewById(R.id.floatingActionButton8);
        btnRequestPickup3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                place1 = true;
                getMyLocation();

            }
        });


        btnRequestPickup4 = (FloatingActionButton) findViewById(R.id.floatingActionButton7);
        btnRequestPickup4.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                place1 = false;
                getMyLocation();

                if(direction!=null)direction.remove();
                if(!(latLng2==null)){
                    Common.currenLocation1=null;
                    getDirection();
                }

            }
        });



        etFinalPickup.setOnFocusChangeListener(new View.OnFocusChangeListener() {
            @Override
            public void onFocusChange(View view, boolean b) {
                if (b) {
                    llPickupInput.setVisibility(View.VISIBLE);
                    llPickupPlace.setVisibility(View.GONE);
                    llDestinationInput.setVisibility(View.GONE);
                    llDestinationPlace.setVisibility(View.GONE);
                    etPickup.requestFocus();
                }
            }
        });
        etFinalDestination.setOnFocusChangeListener(new View.OnFocusChangeListener() {
            @Override
            public void onFocusChange(View view, boolean b) {
                if (b) {
                    llPickupInput.setVisibility(View.GONE);
                    llPickupPlace.setVisibility(View.GONE);
                    llDestinationInput.setVisibility(View.VISIBLE);
                    llDestinationPlace.setVisibility(View.GONE);
                    etDestination.requestFocus();
                }
            }
        });
        etPickup.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                getPlacesByString(charSequence.toString(), true);
            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });
        etDestination.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                getPlacesByString(charSequence.toString(), false);
            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });
        updateFirebaseToken();

    }

    public void vikacar(View view) {
        try {

            btnRequestPickup.setText("REQUEST");
            driverFound = false;
            ca = true;
            tu = false;
            moto = false;
            carr.setImageResource(R.drawable.car_cui3);
            motorb.setImageResource(R.drawable.car_vip_select3);
            tukk.setImageResource(R.drawable.tuk);
            if (mBottomSheet.finalCalculate2 != null)
                car.setText(mBottomSheet.finalCalculate2);
            else
                car.setText("Vika Car request pickup");
            motorbike.setText("Vika Motorbike request pickup");
            tuk.setText("Vika tuk tuk request pickup");

            if (!(Common.currenLocation1 == null)) {
                loadAllAvailableDriver(new LatLng(lat1, lng1));
            } else {
                loadAllAvailableDriver(new LatLng(currentLat, currentLng));
            }
        } catch (Exception e) {
        }

    }

    public void vikatuk(View view) {
        try {

            btnRequestPickup.setText("REQUEST");
            driverFound = false;
            ca = false;
            moto = false;
            tu = true;
            carr.setImageResource(R.drawable.car_cui_select3);
            motorb.setImageResource(R.drawable.car_vip_select3);
            tukk.setImageResource(R.drawable.tuk1);
            car.setText("Vika Car request pickup");
            motorbike.setText("Vika Motorbike request pickup");
            if (mBottomSheet.finalCalculate1 != null)
                tuk.setText(mBottomSheet.finalCalculate1);
            else
                tuk.setText("Vika tuk tuk request pickup");


            if (!(Common.currenLocation1 == null)) {
                loadAllAvailableDriver(new LatLng(lat1, lng1));
            } else {
                loadAllAvailableDriver(new LatLng(currentLat, currentLng));
            }
        } catch (Exception e) {
        }

    }

    public void vikamoto(View view) {
        try {

            btnRequestPickup.setText("REQUEST");
            driverFound = false;
            moto = true;
            ca = false;
            tu = false;
            carr.setImageResource(R.drawable.car_cui_select3);
            motorb.setImageResource(R.drawable.car_vip3);
            tukk.setImageResource(R.drawable.tuk);
            car.setText("Vika Car request pickup");
            if (mBottomSheet.finalCalculate != null)
                motorbike.setText(mBottomSheet.finalCalculate);
            else
                motorbike.setText("Vika Motorbike request pickup");
            tuk.setText("Vika tuk tuk request pickup");


            if (!(Common.currenLocation1 == null)) {
                loadAllAvailableDriver(new LatLng(lat1, lng1));
            } else {
                loadAllAvailableDriver(new LatLng(currentLat, currentLng));
            }
        } catch (Exception e) {
        }
    }

    private void initViews() {
        llPickupInput = findViewById(R.id.ll_pickup_input);
        llPickupPlace = findViewById(R.id.ll_pickup_place);
        llDestinationInput = findViewById(R.id.ll_destination_input);
        car = findViewById(R.id.text_view_id1);
        motorbike = findViewById(R.id.text_view_id2);
        tuk = findViewById(R.id.text_view_id3);
        llDestinationPlace = findViewById(R.id.ll_destination_place);
        etFinalPickup = findViewById(R.id.et_final_pickup_location);
        etFinalDestination = findViewById(R.id.et_final_destination);
        etDestination = findViewById(R.id.et_destination);
        etPickup = findViewById(R.id.et_pickup);
        rvPickupPlaces = findViewById(R.id.rv_pickup_places);
        rvPickupPlaces.setLayoutManager(new LinearLayoutManager(this, LinearLayoutManager.VERTICAL, false));
        rvDestinationPlaces = findViewById(R.id.rv_destination_places);
        rvDestinationPlaces.setLayoutManager(new LinearLayoutManager(this, LinearLayoutManager.VERTICAL, false));

        toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
    }

    public void initDrawer() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.addDrawerListener(toggle);
        toggle.syncState();

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);

        View navigationHeaderView = navigationView.getHeaderView(0);
        TextView tvName = (TextView) navigationHeaderView.findViewById(R.id.tvRiderName);
        TextView tvStars = (TextView) findViewById(R.id.tvStars);
        CircleImageView imageAvatar = (CircleImageView) navigationHeaderView.findViewById(R.id.imgAvatar);

        tvName.setText(Common.currentUser.getName());
        if (Common.currentUser.getRates() != null &&
                !TextUtils.isEmpty(Common.currentUser.getRates()))
            tvStars.setText(Common.currentUser.getRates());

        if (isLoggedInFacebook)
            Picasso.get().load("https://graph.facebook.com/" + Common.userID + "/picture?width=500&height=500").into(imageAvatar);
        else if (account != null)
            Picasso.get().load(account.getPhotoUrl()).into(imageAvatar);
        if (Common.currentUser.getAvatarUrl() != null &&
                !TextUtils.isEmpty(Common.currentUser.getAvatarUrl()))
            Picasso.get().load(Common.currentUser.getAvatarUrl()).into(imageAvatar);
    }

    private void loadUser() {
        FirebaseDatabase.getInstance().getReference(Common.user_rider_tbl)
                .child(Common.userID)
                .addListenerForSingleValueEvent(new ValueEventListener() {
                    @Override
                    public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                        Common.currentUser = dataSnapshot.getValue(User.class);
                        initDrawer();
                    }

                    @Override
                    public void onCancelled(@NonNull DatabaseError databaseError) {

                    }
                });
    }

    private void verifyGoogleAccount() {
        GoogleSignInOptions gso = new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN).requestEmail().build();
        mGoogleApiClient = new GoogleApiClient.Builder(this)
                .enableAutoManage(this, this)
                .addApi(Auth.GOOGLE_SIGN_IN_API, gso)
                .build();
        OptionalPendingResult<GoogleSignInResult> opr = Auth.GoogleSignInApi.silentSignIn(mGoogleApiClient);
        if (opr.isDone()) {
            GoogleSignInResult result = opr.get();
            handleSignInResult(result);
        } else {
            opr.setResultCallback(new ResultCallback<GoogleSignInResult>() {
                @Override
                public void onResult(@NonNull GoogleSignInResult googleSignInResult) {
                    handleSignInResult(googleSignInResult);
                }
            });
        }
    }

    private void updateFirebaseToken() {
        FirebaseDatabase db = FirebaseDatabase.getInstance();
        final DatabaseReference tokens = db.getReference(Common.token_tbl);


        FirebaseMessaging.getInstance().getToken()
                .addOnCompleteListener(new OnCompleteListener<String>() {
                    @Override
                    public void onComplete(@NonNull Task<String> task) {
                        if (!task.isSuccessful()) {
                            Log.w("FIREBASE_TOKEN", "Fetching FCM registration token failed", task.getException());
                            return;
                        }
                        String token = task.getResult();
                        tokens.child(Objects.requireNonNull(FirebaseAuth.getInstance().getUid())).setValue(token);
                    }
                });
    }

    private void requestPickup(String uid) {
        DatabaseReference dbRequest = FirebaseDatabase.getInstance().getReference(Common.pickup_request_tbl);
        GeoFire mGeofire = new GeoFire(dbRequest);
        if (!(Common.currenLocation1 == null)) {
            mGeofire.setLocation(uid, new GeoLocation(Common.currenLocation1.latitude, Common.currenLocation1.longitude),
                    new GeoFire.CompletionListener() {
                        @Override
                        public void onComplete(String key, DatabaseError error) {

                        }
                    });
            if (riderMarket.isVisible()) riderMarket.remove();
            riderMarket = mMap.addMarker(new MarkerOptions().title(getResources().getString(R.string.pickup_here))
                    .snippet("").position(new LatLng(Common.currenLocation1.latitude, Common.currenLocation1.longitude))
                    .icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_RED)));

        } else {
            mGeofire.setLocation(uid, new GeoLocation(Common.currenLocation.latitude, Common.currenLocation.longitude),
                    new GeoFire.CompletionListener() {
                        @Override
                        public void onComplete(String key, DatabaseError error) {

                        }
                    });
            if (riderMarket.isVisible()) riderMarket.remove();
            riderMarket = mMap.addMarker(new MarkerOptions().title(getResources().getString(R.string.pickup_here))
                    .snippet("").position(new LatLng(Common.currenLocation.latitude, Common.currenLocation.longitude))
                    .icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_RED)));

        }
        Objects.requireNonNull(riderMarket).showInfoWindow();
        btnRequestPickup.setText(getResources().getString(R.string.getting_uber));
        if (ca) {


        }
        if (moto) {


        }
        if (tu) {


        }
        findDriver();


    }

    private void findDriver() {
        DatabaseReference driverLocation;
        if (!(Common.currenLocation1 == null)) {

            driverLocation = null;
            if (ca) {
                driverLocation = FirebaseDatabase.getInstance().getReference(Common.driver_tbl).child("Vika Car");
                c = "CAR";
            }
            if (moto) {
                driverLocation = FirebaseDatabase.getInstance().getReference(Common.driver_tbl).child("Vika Motorbike");
                c = "MOTORBIKE";
            }
            if (tu) {
                driverLocation = FirebaseDatabase.getInstance().getReference(Common.driver_tbl).child("Vika Tuk Tuk");
                c = "TUK TUK";

            }
            GeoFire geoFire = new GeoFire(driverLocation);
            GeoQuery geoQuery = geoFire.queryAtLocation(new GeoLocation(Common.currenLocation1.latitude, Common.currenLocation1.longitude), radius);
            geoQuery.removeAllListeners();
            geoQuery.addGeoQueryEventListener(new GeoQueryEventListener() {
                @Override
                public void onKeyEntered(String key, GeoLocation location) {

                    if (!driverFound) {
                        driverFound = true;
                        Common.driverID = key;
                        btnRequestPickup.setText(getApplicationContext().getResources().getString(R.string.call_driver));
                        if (ca) {

                        }

                        if (moto) {


                        }

                        if (tu) {


                        }

                    }
                }

                @Override
                public void onKeyExited(String key) {

                }

                @Override
                public void onKeyMoved(String key, GeoLocation location) {

                }

                @Override
                public void onGeoQueryReady() {
                    if (!driverFound && radius < LIMIT) {
                        radius = radius + 0.5;
                        findDriver();
                    } else {
                        if (!driverFound) {
                            Toast.makeText(HomeActivity.this, "No available any " + c + " driver near you", Toast.LENGTH_SHORT).show();
                            btnRequestPickup.setText("REQUEST");
                            car.setText("Vika car request pickup");
                            motorbike.setText("Vika Motorbike request pickup");
                            tuk.setText("Vika tuk tuk request pickup");
                        }
                    }

                }

                @Override
                public void onGeoQueryError(DatabaseError error) {

                }
            });

        } else {


            driverLocation = null;
            if (ca) {
                driverLocation = FirebaseDatabase.getInstance().getReference(Common.driver_tbl).child("Vika Car");
                c = "CAR";
            }
            if (moto) {
                driverLocation = FirebaseDatabase.getInstance().getReference(Common.driver_tbl).child("Vika Motorbike");
                c = "MOTORBIKE";
            }
            if (tu) {
                driverLocation = FirebaseDatabase.getInstance().getReference(Common.driver_tbl).child("Vika Tuk Tuk");
                c = "TUK TUK";

            }
            GeoFire geoFire = new GeoFire(driverLocation);
            GeoQuery geoQuery = geoFire.queryAtLocation(new GeoLocation(Common.currenLocation.latitude, Common.currenLocation.longitude), radius);
            geoQuery.removeAllListeners();
            geoQuery.addGeoQueryEventListener(new GeoQueryEventListener() {
                @Override
                public void onKeyEntered(String key, GeoLocation location) {

                    if (!driverFound) {
                        driverFound = true;
                        Common.driverID = key;
                        btnRequestPickup.setText(getApplicationContext().getResources().getString(R.string.call_driver));
                        if (ca) {

                        }

                        if (moto) {


                        }

                        if (tu) {


                        }

                    }
                }

                @Override
                public void onKeyExited(String key) {

                }

                @Override
                public void onKeyMoved(String key, GeoLocation location) {

                }

                @Override
                public void onGeoQueryReady() {
                    if (!driverFound && radius < LIMIT) {
                        radius = radius + 0.5;
                        findDriver();
                    } else {
                        if (!driverFound) {
                            Toast.makeText(HomeActivity.this, "No any available " + c + " driver is near you", Toast.LENGTH_SHORT).show();
                            btnRequestPickup.setText("REQUEST");
                            car.setText("Vika car request pickup");
                            motorbike.setText("Vika Motorbike request pickup");
                            tuk.setText("Vika tuk tuk request pickup");
                        }
                    }

                }

                @Override
                public void onGeoQueryError(DatabaseError error) {

                }
            });

        }


    }

    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            super.onBackPressed();
        }
    }

    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        // Handle navigation view item clicks here.
        int id = item.getItemId();

        switch (id) {
            case R.id.nav_trip_history:
                showTripHistory();
                break;
            case R.id.nav_updateInformation:
                showDialogUpdateInfo();
                break;
            case R.id.nav_signOut:
                signOut();
                break;
        }

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }

    private void showTripHistory() {
        Intent intent = new Intent(HomeActivity.this, TripHistoryActivity.class);
        startActivity(intent);
    }

    private void showDialogUpdateInfo() {
        AlertDialog.Builder alertDialog = new AlertDialog.Builder(HomeActivity.this);
        alertDialog.setTitle("UPDATE INFORMATION");
        LayoutInflater inflater = this.getLayoutInflater();
        View layout_pwd = inflater.inflate(R.layout.layout_update_information, null);
        final MaterialEditText etName = (MaterialEditText) layout_pwd.findViewById(R.id.etName);
        final MaterialEditText etPhone = (MaterialEditText) layout_pwd.findViewById(R.id.etPhone);
        final ImageView image_upload = (ImageView) layout_pwd.findViewById(R.id.imageUpload);
        image_upload.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String ava2 = "avatarUrl";
                chooseImage1(ava2);
            }
        });
        alertDialog.setView(layout_pwd);
        alertDialog.setPositiveButton("UPDATE", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                dialogInterface.dismiss();
                final android.app.AlertDialog waitingDialog = new SpotsDialog.Builder().setContext(HomeActivity.this).build();
                waitingDialog.show();
                String name = Objects.requireNonNull(etName.getText()).toString();
                String phone = Objects.requireNonNull(etPhone.getText()).toString();

                Map<String, Object> updateInfo = new HashMap<>();
                if (!TextUtils.isEmpty(name))
                    updateInfo.put("name", name);
                if (!TextUtils.isEmpty(phone))
                    updateInfo.put("phone", phone);
                DatabaseReference driverInformation = FirebaseDatabase.getInstance().getReference(Common.user_rider_tbl);
                driverInformation.child(Common.userID)
                        .updateChildren(updateInfo)
                        .addOnCompleteListener(new OnCompleteListener<Void>() {
                            @Override
                            public void onComplete(@NonNull Task<Void> task) {
                                waitingDialog.dismiss();
                                if (task.isSuccessful())
                                    Toast.makeText(HomeActivity.this, "Information Updated!", Toast.LENGTH_SHORT).show();
                                else
                                    Toast.makeText(HomeActivity.this, "Information Update Failed!", Toast.LENGTH_SHORT).show();

                            }
                        });
            }
        });
        alertDialog.setNegativeButton("CANCEL", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                dialogInterface.dismiss();
            }
        });
        alertDialog.show();
    }

    private void chooseImage() {
        Dexter.withActivity(this)
                .withPermissions(Manifest.permission.READ_EXTERNAL_STORAGE,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE)
                .withListener(new MultiplePermissionsListener() {
                    @Override
                    public void onPermissionsChecked(MultiplePermissionsReport report) {
                        if (report.areAllPermissionsGranted()) {
                            Intent intent = new Intent(Intent.ACTION_PICK);
                            intent.setType("image/*");
                            startActivityForResult(intent, Common.PICK_IMAGE_REQUEST);
                        } else {
                            Toast.makeText(getApplicationContext(), "Permission denied", Toast.LENGTH_SHORT).show();
                        }
                    }

                    @Override
                    public void onPermissionRationaleShouldBeShown(List<PermissionRequest> permissions, PermissionToken token) {
                        token.continuePermissionRequest();
                    }
                }).check();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == Common.PICK_IMAGE_REQUEST && resultCode == RESULT_OK && data != null && data.getData() != null) {
            Uri saveUri = data.getData();
            if (saveUri != null) {
                final ProgressDialog progressDialog = new ProgressDialog(this);
                progressDialog.setMessage("Uploading...");
                progressDialog.show();

                String imageName = UUID.randomUUID().toString();
                final StorageReference imageFolder = storageReference.child("images/" + imageName);

                imageFolder.putFile(saveUri)
                        .addOnSuccessListener(new OnSuccessListener<UploadTask.TaskSnapshot>() {
                            @Override
                            public void onSuccess(UploadTask.TaskSnapshot taskSnapshot) {
                                progressDialog.dismiss();
                                Toast.makeText(HomeActivity.this, "Uploaded!", Toast.LENGTH_SHORT).show();
                                imageFolder.getDownloadUrl().addOnSuccessListener(new OnSuccessListener<Uri>() {
                                    @Override
                                    public void onSuccess(Uri uri) {

                                        Map<String, Object> avatarUpdate = new HashMap<>();
                                        avatarUpdate.put(avata, uri.toString());


                                        DatabaseReference driverInformations = FirebaseDatabase.getInstance().getReference(Common.user_rider_tbl);
                                        driverInformations.child(Common.userID).updateChildren(avatarUpdate)
                                                .addOnCompleteListener(new OnCompleteListener<Void>() {
                                                    @Override
                                                    public void onComplete(@NonNull Task<Void> task) {
                                                        if (task.isSuccessful())
                                                            Toast.makeText(HomeActivity.this, "Uploaded!", Toast.LENGTH_SHORT).show();
                                                        else
                                                            Toast.makeText(HomeActivity.this, "Uploaded error!", Toast.LENGTH_SHORT).show();

                                                    }
                                                });
                                    }
                                });
                            }
                        }).addOnProgressListener(new OnProgressListener<UploadTask.TaskSnapshot>() {
                            @Override
                            public void onProgress(@NonNull UploadTask.TaskSnapshot taskSnapshot) {
                                double progress = (100.0 * taskSnapshot.getBytesTransferred()) / taskSnapshot.getTotalByteCount();
                                progressDialog.setMessage("Uploaded " + progress + "%");
                            }
                        });
            }
        }
    }

    private void signOut() {
        if (account != null) {
            Auth.GoogleSignInApi.signOut(mGoogleApiClient).setResultCallback(new ResultCallback<Status>() {
                @Override
                public void onResult(@NonNull Status status) {
                    if (status.isSuccess()) {
                        Intent intent = new Intent(HomeActivity.this, LoginActivity.class);
                        startActivity(intent);
                        finish();
                    } else {
                        Toast.makeText(HomeActivity.this, "Could not log out", Toast.LENGTH_SHORT).show();
                    }
                }
            });
        } else if (isLoggedInFacebook) {
            LoginManager.getInstance().logOut();
            Intent intent = new Intent(HomeActivity.this, LoginActivity.class);
            startActivity(intent);
            finish();
        } else {
            FirebaseAuth.getInstance().signOut();
            Intent intent = new Intent(HomeActivity.this, LoginActivity.class);
            startActivity(intent);
            finish();
        }
    }

    private void handleSignInResult(GoogleSignInResult result) {
        if (result.isSuccess()) {
            account = result.getSignInAccount();
            Common.userID = Objects.requireNonNull(account).getId();
            loadUser();
        } else if (isLoggedInFacebook) {
            GraphRequest request = GraphRequest.newMeRequest(
                    accessToken,
                    new GraphRequest.GraphJSONObjectCallback() {
                        @Override
                        public void onCompleted(JSONObject object, GraphResponse response) {
                            String id = object.optString("id");
                            Common.userID = id;
                            loadUser();
                        }
                    });
            request.executeAsync();
        } else {
            Common.userID = Objects.requireNonNull(FirebaseAuth.getInstance().getCurrentUser()).getUid();
            loadUser();
        }
    }

    private void displayLocation() {
        if (currentLat != null && currentLng != null) {
            driversAvailable = FirebaseDatabase.getInstance().getReference(Common.driver_tbl);
            driversAvailable.addValueEventListener(new ValueEventListener() {
                @Override
                public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                    if (!(Common.currenLocation1 == null)) {
                        loadAllAvailableDriver(new LatLng(lat1, lng1));
                    } else {
                        loadAllAvailableDriver(new LatLng(currentLat, currentLng));
                    }
                }

                @Override
                public void onCancelled(@NonNull DatabaseError databaseError) {

                }
            });

            if (!(Common.currenLocation1 == null)) {
                loadAllAvailableDriver(new LatLng(lat1, lng1));
            } else {
                loadAllAvailableDriver(new LatLng(currentLat, currentLng));
            }

        } else {
            ShowMessage.messageError(this, Errors.WITHOUT_LOCATION);
        }

    }


    private void displayLocation2() {
        if (currentLat != null && currentLng != null) {
            driversAvailable = FirebaseDatabase.getInstance().getReference(Common.driver_tbl);
            driversAvailable.addValueEventListener(new ValueEventListener() {
                @Override
                public void onDataChange(@NonNull DataSnapshot dataSnapshot) {

                    loadAllAvailableDriver(new LatLng(currentLat, currentLng));

                }

                @Override
                public void onCancelled(@NonNull DatabaseError databaseError) {

                }
            });


            loadAllAvailableDriver(new LatLng(currentLat, currentLng));


        } else {
            ShowMessage.messageError(this, Errors.WITHOUT_LOCATION);
        }

    }


    public void chooseImage1(String ava) {
        avata = ava;
        chooseImage();
    }

    private void loadAllAvailableDriver(final LatLng location) {
        for (Marker driverMarker : driverMarkers) {
            driverMarker.remove();
        }
        driverMarkers.clear();
        if (!pickupPlacesSelected) {
            if (riderMarket != null)
                riderMarket.remove();

            riderMarket = mMap.addMarker(new MarkerOptions().position(location)
                    .title(getResources().getString(R.string.you))
                    .icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_RED)));
            mMap.animateCamera(CameraUpdateFactory.newLatLngZoom(location, 15.80f));
        }


        DatabaseReference driverLocation;
        driverLocation = null;
        if (ca) {
            driverLocation = FirebaseDatabase.getInstance().getReference(Common.driver_tbl).child("Vika Car");
        }
        if (moto)
            driverLocation = FirebaseDatabase.getInstance().getReference(Common.driver_tbl).child("Vika Motorbike");

        if (tu)
            driverLocation = FirebaseDatabase.getInstance().getReference(Common.driver_tbl).child("Vika Tuk Tuk");

        GeoFire geoFire = new GeoFire(driverLocation);

        GeoQuery geoQuery = geoFire.queryAtLocation(new GeoLocation(location.latitude, location.longitude), distance);
        geoQuery.removeAllListeners();
        geoQuery.addGeoQueryEventListener(new GeoQueryEventListener() {
            @Override
            public void onKeyEntered(String key, final GeoLocation location) {
                FirebaseDatabase.getInstance().getReference(Common.user_driver_tbl).child(key).addListenerForSingleValueEvent(new ValueEventListener() {
                    @Override
                    public void onDataChange(@NonNull DataSnapshot dataSnapshot) {

                        User driver = dataSnapshot.getValue(User.class);
                        String name;
                        String phone;

                        if (Objects.requireNonNull(driver).getName() != null)
                            name = driver.getName();
                        else name = "not available";

                        if (driver.getPhone() != null) phone = "Phone: " + driver.getPhone();
                        else phone = "Phone: none";

                        if (ca) {
                            driverMarkers.add(mMap.addMarker(new MarkerOptions().position(new LatLng(location.latitude, location.longitude)).flat(true)
                                    .title(name).snippet("Driver ID: " + dataSnapshot.getKey()).icon(BitmapDescriptorFactory.fromResource(R.drawable.car))));
                        }

                        if (moto) {
                            driverMarkers.add(mMap.addMarker(new MarkerOptions().position(new LatLng(location.latitude, location.longitude)).flat(true)
                                    .title(name).snippet("Driver ID: " + dataSnapshot.getKey()).icon(BitmapDescriptorFactory.fromResource(R.drawable.moto4))));
                        }

                        if (tu) {
                            driverMarkers.add(mMap.addMarker(new MarkerOptions().position(new LatLng(location.latitude, location.longitude)).flat(true)
                                    .title(name).snippet("Driver ID: " + dataSnapshot.getKey()).icon(BitmapDescriptorFactory.fromResource(R.drawable.tuk3))));
                        }


                    }

                    @Override
                    public void onCancelled(@NonNull DatabaseError databaseError) {

                    }
                });
            }

            @Override
            public void onKeyExited(String key) {

            }

            @Override
            public void onKeyMoved(String key, GeoLocation location) {

            }

            @Override
            public void onGeoQueryReady() {
                if (distance <= LIMIT) {
                    distance = distance + 0.5;
                    loadAllAvailableDriver(location);
                }
            }

            @Override
            public void onGeoQueryError(DatabaseError error) {

            }
        });
    }

    @Override
    public void onMapReady(GoogleMap googleMap) {
        mMap = googleMap;
        View mapView = null;
        int zoom = (int) mMap.getCameraPosition().zoom;

        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED && ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            // TODO: Consider calling
            //    ActivityCompat#requestPermissions
            // here to request the missing permissions, and then overriding
            //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
            //                                          int[] grantResults)
            // to handle the case where the user grants the permission. See the documentation
            // for ActivityCompat#requestPermissions for more details.
            return;
        }
        mMap.setMyLocationEnabled(true);
        //locationButton();
        mMap.setBuildingsEnabled(true);
        mMap.getUiSettings().setMyLocationButtonEnabled(false);
        mMap.getUiSettings().setCompassEnabled(true);
        mMap.setInfoWindowAdapter(new CustomInfoWindow(this));
        googleMap.setMapStyle(MapStyleOptions.loadRawResourceStyle(this, R.raw.uber_style_map));

        //googleMap.setMapStyle(MapStyleOptions.loadRawResourceStyle(this, R.raw.map_style));

        mMap.setOnMapClickListener(new GoogleMap.OnMapClickListener() {
            @Override
            public void onMapClick(@NonNull LatLng latLng) {
                if (destinationMarker != null)
                    destinationMarker.remove();
                destinationMarker = mMap.addMarker(new MarkerOptions().position(latLng)
                        .icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_RED))
                        .title("Destination"));
                mMap.animateCamera(CameraUpdateFactory.newLatLngZoom(latLng, 15.80f));
                latLng2=latLng;
                try {
                    geocoder = new Geocoder(HomeActivity.this, Locale.ENGLISH);
                    addresses = geocoder.getFromLocation(latLng2.latitude, latLng2.longitude, 1);

                    if (addresses != null && addresses.size() > 0) {
                        String address = addresses.get(0).getAddressLine(0);
                        String city = addresses.get(0).getLocality();
                        String state = addresses.get(0).getAdminArea();
                        String country = addresses.get(0).getCountryName();
                        String postalCode = addresses.get(0).getPostalCode();
                        String knownName = addresses.get(0).getFeatureName();

                        etFinalDestination.setText(address);

                        //locationTxt.setText(address + " " + city + " " + country);
                    }
                } catch (IOException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }

                mBottomSheet = BottomSheetRiderFragment.newInstance(String.format("%f,%f", currentLat, currentLng), String.format("%f,%f", latLng.latitude, latLng.longitude), true);
                mBottomSheet.show(getSupportFragmentManager(), mBottomSheet.getTag());
                mBottomSheet.dismiss();

                if (riderMarket2!=null) riderMarket2.remove();

                if(direction!=null)direction.remove();
                if(!(latLng2==null)){
                    getDirection();
                }
            }
        });
        mMap.setOnInfoWindowClickListener(this);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        location.onRequestPermissionResult(requestCode, permissions, grantResults);
    }

    @Override
    protected void onStop() {
        super.onStop();
        location.stopUpdateLocation();
    }

    @Override
    protected void onStart() {
        super.onStart();
        displayLocation();
        location.inicializeLocation();
    }

    @Override
    public void onConnected(@Nullable Bundle bundle) {

    }

    @Override
    public void onConnectionSuspended(int i) {
        mGoogleApiClient.connect();
    }

    @Override
    public void onConnectionFailed(@NonNull ConnectionResult connectionResult) {

    }

    @Override
    public void onInfoWindowClick(Marker marker) {

        if (!(Common.currenLocation1 == null)) {
            if (!(marker.getTitle()).equals("You") && !(marker.getTitle()).equals("Destination")) {
                findDriver();
                Intent intent = new Intent(HomeActivity.this, CallDriverActivity.class);
                String ID = (marker.getSnippet()).replace("Driver ID: ", "");
                intent.putExtra("driverID", ID);
                intent.putExtra("lat", lat1);
                intent.putExtra("lng", lng1);
                startActivity(intent);
            }
        } else {
            if (!(marker.getTitle()).equals("You") && !(marker.getTitle()).equals("Destination")) {
                findDriver();
                Intent intent = new Intent(HomeActivity.this, CallDriverActivity.class);
                String ID = (marker.getSnippet()).replace("Driver ID: ", "");
                intent.putExtra("driverID", ID);
                intent.putExtra("lat", currentLat);
                intent.putExtra("lng", currentLng);
                startActivity(intent);
            }
        }

    }

    private void getPlacesByString(String s, final boolean isPickup) {

        String queryEncode = s.toString();
        if (!(Common.currenLocation1 == null)) {
            try {
                queryEncode = URLEncoder.encode(s.toString(), "UTF-8");
            } catch (UnsupportedEncodingException e) {
                e.printStackTrace();
            }
            LatLng Tesni = Common.currenLocation1;
            String query = "&query=" + queryEncode;
            String location = "&location=" + Tesni.latitude + "," + Tesni.longitude;
            String radius = "radius=1500";
            String key = "&key=" + ConfigApp.GOOGLE_API_KEY;
            String url = (URL_BASE_API_PLACES + radius + location + query + key).replaceAll(" ", "%20");

            Log.d("URL_PLACES", url);
            networkUtil.httpRequest(url, new HttpResponse() {
                @Override
                public void httpResponseSuccess(String response) {
                    pickupPlacesSelected = true;
                    Gson gson = new Gson();
                    PlacesResponse placesResponse = gson.fromJson(response, PlacesResponse.class);
                    for (Results result : placesResponse.results) {
                        if (result.geometry.location == null) {
                            placesResponse.results.remove(result);
                        } else if (result.geometry.location.lat == null || result.geometry.location.lat.equals("") || result.geometry.location.lat.equals("0.0")) {
                            placesResponse.results.remove(result);
                        } else if (result.geometry.location.lng == null || result.geometry.location.lng.equals("") || result.geometry.location.lng.equals("0.0")) {
                            placesResponse.results.remove(result);
                        }
                    }
                    if (isPickup)
                        implementPickupRecyclerView(placesResponse.results);
                    else
                        implementDestinationRecyclerView(placesResponse.results);

                }
            });


        } else {

            try {
                queryEncode = URLEncoder.encode(s.toString(), "UTF-8");
            } catch (UnsupportedEncodingException e) {
                e.printStackTrace();
            }
            LatLng Tesni = Common.currenLocation;
            String query = "&query=" + queryEncode;
            String location = "&location=" + Tesni.latitude + "," + Tesni.longitude;
            String radius = "radius=1500";
            String key = "&key=" + ConfigApp.GOOGLE_API_KEY;
            String url = (URL_BASE_API_PLACES + radius + location + query + key).replaceAll(" ", "%20");

            Log.d("URL_PLACES", url);
            networkUtil.httpRequest(url, new HttpResponse() {
                @Override
                public void httpResponseSuccess(String response) {
                    pickupPlacesSelected = true;
                    Gson gson = new Gson();
                    PlacesResponse placesResponse = gson.fromJson(response, PlacesResponse.class);
                    for (Results result : placesResponse.results) {
                        if (result.geometry.location == null) {
                            placesResponse.results.remove(result);
                        } else if (result.geometry.location.lat == null || result.geometry.location.lat.equals("") || result.geometry.location.lat.equals("0.0")) {
                            placesResponse.results.remove(result);
                        } else if (result.geometry.location.lng == null || result.geometry.location.lng.equals("") || result.geometry.location.lng.equals("0.0")) {
                            placesResponse.results.remove(result);
                        }
                    }
                    if (isPickup)
                        implementPickupRecyclerView(placesResponse.results);
                    else
                        implementDestinationRecyclerView(placesResponse.results);

                }
            });

        }

    }

    private void implementPickupRecyclerView(final ArrayList<Results> results) {
        PlacesAdapter placesAdapter = new PlacesAdapter(this, results, new ClickListener() {
            @Override
            public void onClick(View view, int index) {
                mPlaceLocation = results.get(index).formatted_address;
                etFinalPickup.setText(mPlaceLocation);
                place1 = true;
                llPickupInput.setVisibility(View.GONE);
                llPickupPlace.setVisibility(View.VISIBLE);
                llDestinationInput.setVisibility(View.GONE);
                llDestinationPlace.setVisibility(View.VISIBLE);

                lat1 = Double.valueOf(results.get(index).geometry.location.lat);
                lng1 = Double.valueOf(results.get(index).geometry.location.lng);
                LatLng latLng = new LatLng(lat1, lng1);
                Common.currenLocation1 = latLng;
                if (riderMarket != null)
                    riderMarket.remove();

                loadAllAvailableDriver(new LatLng(lat1, lng1));
                riderMarket = mMap.addMarker(new MarkerOptions().position(latLng)
                        .icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_RED))
                        .title("Pickup Here"));
                mMap.animateCamera(CameraUpdateFactory.newLatLngZoom(latLng, 15.80f));

                if (riderMarket2!=null) riderMarket2.remove();

                if(direction!=null)direction.remove();
                if(!(latLng2==null)){
                getDirection();
                }

            }
        });
        rvPickupPlaces.setAdapter(placesAdapter);
    }

    private void implementDestinationRecyclerView(final ArrayList<Results> results) {
        PlacesAdapter placesAdapter = new PlacesAdapter(this, results, new ClickListener() {
            @Override
            public void onClick(View view, int index) {
                mPlaceDestination = results.get(index).formatted_address;
                etFinalDestination.setText(mPlaceDestination);

                llPickupInput.setVisibility(View.GONE);
                llPickupPlace.setVisibility(View.VISIBLE);
                llDestinationInput.setVisibility(View.GONE);
                llDestinationPlace.setVisibility(View.VISIBLE);

                Double lat = Double.valueOf(results.get(index).geometry.location.lat);
                Double lng = Double.valueOf(results.get(index).geometry.location.lng);
                latLng2 = new LatLng(lat, lng);
                if (destinationMarker != null)
                    destinationMarker.remove();
                destinationMarker = mMap.addMarker(new MarkerOptions().position(latLng2)
                        .icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_RED))
                        .title("Destination"));
                mMap.animateCamera(CameraUpdateFactory.newLatLngZoom(latLng2, 15.80f));

                mBottomSheet = BottomSheetRiderFragment.newInstance(mPlaceLocation, mPlaceDestination, true);
                //mBottomSheet.getPrice1(mPlaceLocation, mPlaceDestination);
                mBottomSheet.show(getSupportFragmentManager(), mBottomSheet.getTag());
                mBottomSheet.dismiss();

                if (riderMarket2!=null) riderMarket2.remove();

                if(direction!=null)direction.remove();
                if(!(latLng2==null)){
                    getDirection();
                }
            }
        });
        rvDestinationPlaces.setAdapter(placesAdapter);
    }


    private void loadcurrent() {
        try {
            geocoder = new Geocoder(HomeActivity.this, Locale.ENGLISH);
            addresses = geocoder.getFromLocation(Common.currenLocation.latitude, Common.currenLocation.longitude, 1);
            StringBuilder str = new StringBuilder();

            if (Geocoder.isPresent()) {

                android.location.Address returnAddress = addresses.get(0);

                String localityString = returnAddress.getAddressLine(0);


                str.append(localityString).append("");

                if (!(place1 == true)) {
                    etFinalPickup.setText("My current location");
                    loadAllAvailableDriver(new LatLng(Common.currenLocation.latitude, Common.currenLocation.longitude));
                }

                // Toast.makeText(getApplicationContext(), str, Toast.LENGTH_SHORT).show();

            } else {
                Toast.makeText(getApplicationContext(), "geocoder not present", Toast.LENGTH_SHORT).show();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }


    }

    private void CheckMapPermission() {


        if (Build.VERSION.SDK_INT > Build.VERSION_CODES.LOLLIPOP) {

            if (ActivityCompat.checkSelfPermission(HomeActivity.this, android.Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED &&
                    ActivityCompat.checkSelfPermission(HomeActivity.this, android.Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
                ActivityCompat.requestPermissions(HomeActivity.this, new String[]{android.Manifest.permission.ACCESS_FINE_LOCATION, Manifest.permission.ACCESS_COARSE_LOCATION}, 1002);
            } else {

                setupLocationManager();
            }
        } else {
            setupLocationManager();
        }

    }

    private void setupLocationManager() {
        //buildGoogleApiClient();
        if (mGoogleApiClient == null) {

            mGoogleApiClient = new GoogleApiClient.Builder(this)
                    .addConnectionCallbacks(this)
                    .addOnConnectionFailedListener(this)
                    .addApi(LocationServices.API)
                    .addApi(Places.GEO_DATA_API)
                    .addApi(Places.PLACE_DETECTION_API)
                    .build();

        }
        mGoogleApiClient.connect();
        createLocationRequest();
    }

    protected void createLocationRequest() {

        request = new LocationRequest();
        request.setSmallestDisplacement(10);
        request.setFastestInterval(50000);
        request.setPriority(LocationRequest.PRIORITY_HIGH_ACCURACY);
        request.setNumUpdates(3);

        LocationSettingsRequest.Builder builder = new LocationSettingsRequest.Builder()
                .addLocationRequest(request);
        builder.setAlwaysShow(true);

        PendingResult<LocationSettingsResult> result =
                LocationServices.SettingsApi.checkLocationSettings(mGoogleApiClient,
                        builder.build());


        result.setResultCallback(new ResultCallback<LocationSettingsResult>() {
            @Override
            public void onResult(@NonNull LocationSettingsResult result) {
                final Status status = result.getStatus();
                switch (status.getStatusCode()) {

                    case LocationSettingsStatusCodes.SUCCESS:
                        setInitialLocation();
                        break;

                    case LocationSettingsStatusCodes.RESOLUTION_REQUIRED:

                        try {

                            status.startResolutionForResult(
                                    HomeActivity.this,
                                    REQUEST_CHECK_SETTINGS);
                        } catch (IntentSender.SendIntentException e) {

                        }
                        break;

                    case LocationSettingsStatusCodes.SETTINGS_CHANGE_UNAVAILABLE:

                        break;
                }
            }
        });


    }


    private void setInitialLocation() {


        if (ActivityCompat.checkSelfPermission(HomeActivity.this, android.Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED && ActivityCompat.checkSelfPermission(HomeActivity.this, android.Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            // TODO: Consider calling
            //    ActivityCompat#requestPermissions
            // here to request the missing permissions, and then overriding
            //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
            //                                          int[] grantResults)
            // to handle the case where the user grants the permission. See the documentation
            // for ActivityCompat#requestPermissions for more details.
            return;
        }
        LocationServices.FusedLocationApi.requestLocationUpdates(mGoogleApiClient, request, new LocationListener() {
            @Override
            public void onLocationChanged(android.location.Location location) {

                mLastLocation = location;
                double lat = location.getLatitude();
                double lng = location.getLongitude();

                HomeActivity.this.latitude = lat;
                HomeActivity.this.longitude = lng;

                try {
                    if (now != null) {
                        now.remove();
                    }
                    LatLng positionUpdate = new LatLng(HomeActivity.this.latitude, HomeActivity.this.longitude);
                    CameraUpdate update = CameraUpdateFactory.newLatLngZoom(positionUpdate, 15.8f);
                    now = mMap.addMarker(new MarkerOptions().position(positionUpdate)
                            .title("Your Location"));

                    mMap.animateCamera(update);


                } catch (Exception ex) {

                    ex.printStackTrace();
                    Log.e("MapException", ex.getMessage());

                }


                try {
                    geocoder = new Geocoder(HomeActivity.this, Locale.ENGLISH);
                    addresses = geocoder.getFromLocation(latitude, longitude, 1);
                    StringBuilder str = new StringBuilder();
                    if (Geocoder.isPresent()) {

                        android.location.Address returnAddress = addresses.get(0);

                        String localityString = returnAddress.getAddressLine(0);


                        str.append(localityString).append("");

                        Toast.makeText(getApplicationContext(), str,
                                Toast.LENGTH_SHORT).show();

                    } else {

                    }


                } catch (IOException e) {
// TODO Auto-generated catch block

                    Log.e("tag", e.getMessage());
                }


            }

        });
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (mGoogleApiClient.isConnected()) {
            setInitialLocation();

        }

        LocationManager service = (LocationManager) getSystemService(LOCATION_SERVICE);
        boolean enabled = service.isProviderEnabled(LocationManager.GPS_PROVIDER);


        if (!enabled) {
            buildAlertMessageNoGps();
        }
        if (enabled) {


        }


    }


    protected void buildAlertMessageNoGps() {

        final android.app.AlertDialog.Builder builder = new android.app.AlertDialog.Builder(this);
        builder.setMessage("Please Turn ON your GPS Connection")
                .setTitle("GPS Not Enabled")
                .setCancelable(false)
                .setPositiveButton("Yes", new DialogInterface.OnClickListener() {
                    public void onClick(final DialogInterface dialog, final int id) {
                        startActivity(new Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS));
                    }
                })
                .setNegativeButton("No", new DialogInterface.OnClickListener() {
                    public void onClick(final DialogInterface dialog, final int id) {
                        dialog.cancel();
                    }
                });
        final android.app.AlertDialog alert = builder.create();
        alert.show();
    }

    private void locationButton() {

        //MapFragment mapFragment = (MapFragment) getFragmentManager().findFragmentById( R.id.map );

        View locationButton = ((View) mapFragment.getView().findViewById(Integer.parseInt("2"))
                .getParent()).findViewById(Integer.parseInt("2"));
        if (locationButton != null && locationButton.getLayoutParams() instanceof RelativeLayout.LayoutParams) {

            RelativeLayout.LayoutParams params = (RelativeLayout.LayoutParams) locationButton.getLayoutParams();


            params.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
            params.addRule(RelativeLayout.ALIGN_PARENT_LEFT);
            params.addRule(RelativeLayout.ALIGN_PARENT_RIGHT, 0);
            params.addRule(RelativeLayout.ALIGN_PARENT_TOP, 0);


            final int margin = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 150,
                    getResources().getDisplayMetrics());
            params.setMargins(margin, margin, margin, margin);

            locationButton.setLayoutParams(params);
        }

    }

    void hideStatusBar() {
        if (Build.VERSION.SDK_INT < 16) {
            getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                    WindowManager.LayoutParams.FLAG_FULLSCREEN);
        } else {
            View decorView = getWindow().getDecorView();
            int uiOptions = View.SYSTEM_UI_FLAG_FULLSCREEN;
            decorView.setSystemUiVisibility(uiOptions);
        }
    }


    private void getMyLocation() {
        if (!(currentLat==null)) {
            LatLng latLng = Common.currenLocation;
            CameraUpdate cameraUpdate = CameraUpdateFactory.newLatLngZoom(latLng, 15.8f);
            mMap.animateCamera(cameraUpdate);
        }
    }



    private void getDirection() {
        LatLng currentPosition = null;
        if(!(Common.currenLocation1==null)) {
            currentPosition = new LatLng(Common.currenLocation1.latitude, Common.currenLocation1.longitude);
        }
        else {
            if(currentLat!=null){
            currentPosition = new LatLng(currentLat, currentLng);}
        }
        String requestApi = null;
        try {
            requestApi = "https://maps.googleapis.com/maps/api/directions/json?" +
                    "mode=driving&" +
                    "transit_routing_preference=less_driving&" +
                    "origin=" + currentPosition.latitude + "," + currentPosition.longitude + "&" +
                    "destination=" + latLng2.latitude + "," + latLng2.longitude + "&" +
                    "key=" + ConfigApp.GOOGLE_API_KEY;//*/

            Log.d("ISAKAY", requestApi);//print url for debug currentPosition.latitude currentPosition.longitude
            mService.getPath(requestApi).enqueue(new Callback<String>() {
                @Override
                public void onResponse(@NonNull Call<String> call, @NonNull Response<String> response) {
                    try {
                        //Toast.makeText(TrackingActivity.this,(response.body()).toString(),Toast.LENGTH_SHORT).show();
                        new ParserTask().execute(Objects.requireNonNull(response.body()).toString());

                    } catch (Exception e) {
                        e.printStackTrace();
                    }

                }

                @Override
                public void onFailure(@NonNull Call<String> call, @NonNull Throwable t) {
                    Toast.makeText(HomeActivity.this, "" + t.getMessage(), Toast.LENGTH_SHORT).show();

                }
            });
        } catch (Exception e) {
            e.printStackTrace();
            Toast.makeText(HomeActivity.this,e.getMessage(),Toast.LENGTH_SHORT).show();
        }

    }




    private class ParserTask extends AsyncTask<String, Integer, List<List<HashMap<String, String>>>> {

        ProgressDialog mDialog = new ProgressDialog(HomeActivity.this);

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            mDialog.setMessage("Please waiting...");
            mDialog.show();
        }

        @Override
        protected List<List<HashMap<String, String>>> doInBackground(String... strings) {
            JSONObject jsonObject;
            List<List<HashMap<String, String>>> routes = null;

            try {
                jsonObject = new JSONObject(strings[0]);
                DirectionJSONParser parser = new DirectionJSONParser();
                routes = parser.parse(jsonObject);
            } catch (JSONException e) {
                e.printStackTrace();
            }

            return routes;
        }

        @Override
        protected void onPostExecute(List<List<HashMap<String, String>>> lists) {
            mDialog.dismiss();

            ArrayList points = null;
            PolylineOptions polylineOptions = null;

            for (int i = 0; i < lists.size(); i++) {
                points = new ArrayList();
                polylineOptions = new PolylineOptions();

                List<HashMap<String, String>> path = lists.get(i);

                for (int j = 0; j < path.size(); j++) {


                    HashMap<String, String> point = path.get(j);

                    if(j==(path.size()-1))
                    {
                        double latk = Double.parseDouble(Objects.requireNonNull(point.get("lat")));
                        double lngk = Double.parseDouble(Objects.requireNonNull(point.get("lng")));
                        LatLng positionk = new LatLng(latk, lngk);
                        String ikoshh = mBottomSheet.Calculate2;
                        if(moto){if (mBottomSheet.Calculate1 != null)ikoshh=mBottomSheet.Calculate1;}
                        if(ca){if (mBottomSheet.Calculate2 != null)ikoshh=mBottomSheet.Calculate2;}
                        if(tu){if (mBottomSheet.Calculate != null)ikoshh=mBottomSheet.Calculate;}

                      //  if (riderMarket2.isVisible()) riderMarket2.remove();
                        riderMarket2 = mMap.addMarker(new MarkerOptions().title(getResources().getString(R.string.pickup_here2))
                                .snippet(ikoshh).position(positionk).
                                icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_YELLOW)));
                        if(riderMarket2!=null)riderMarket2.showInfoWindow();
                        //LatLng latLng = positionk;
                        CameraUpdate cameraUpdate = CameraUpdateFactory.newLatLngZoom(positionk, 15.8f);
                        mMap.animateCamera(cameraUpdate);
                    }

                    double lat = Double.parseDouble(Objects.requireNonNull(point.get("lat")));
                    double lng = Double.parseDouble(Objects.requireNonNull(point.get("lng")));
                    LatLng position = new LatLng(lat, lng);

                    points.add(position);
                }

                polylineOptions.addAll(points);
                polylineOptions.width(15);
                polylineOptions.color(Color.BLUE);
                polylineOptions.geodesic(true);

            }
            direction = mMap.addPolyline(Objects.requireNonNull(polylineOptions));
            if (!(currentLat==null)) {
                LatLng latLng = Common.currenLocation;
                CameraUpdate cameraUpdate = CameraUpdateFactory.newLatLngZoom(latLng, 13.8f);
                mMap.animateCamera(cameraUpdate);
                try {
                    Thread.sleep(-4000);
                }
               catch (Exception e)
               {

               }

            }
        }
    }




}

/*

 <View
                android:layout_width="match_parent"
                android:layout_height="1dp"
                android:layout_marginRight="16dp"
                android:layout_marginLeft="16dp"
                android:background="#AAA"/>


                 <androidx.appcompat.widget.Toolbar
            android:id="@+id/toolbar"
            android:layout_width="match_parent"
            android:layout_height="?attr/actionBarSize"
            android:background="?attr/colorPrimary"
            app:popupTheme="@style/AppTheme.PopupOverlay" />

            View locationButton = ((View) mapView.findViewById(Integer.parseInt("1")).getParent()).findViewById(Integer.parseInt("2"));
        RelativeLayout.LayoutParams rlp = (RelativeLayout.LayoutParams) locationButton.getLayoutParams();
        // position on right bottom
        rlp.addRule(RelativeLayout.ALIGN_PARENT_TOP, 0);
        rlp.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM, RelativeLayout.TRUE);rlp.setMargins(0,0,30,30);






            <androidx.cardview.widget.CardView
                android:layout_width="match_parent"
                android:layout_height="wrap_content"
                android:layout_marginRight="10dp"
                android:layout_marginLeft="10dp"
                app:cardCornerRadius="20dp"
                android:elevation="10dp">

                <RelativeLayout
                    android:layout_width="match_parent"
                    android:layout_height="wrap_content">

                    <Button
                        android:id="@+id/btnPickupRequest"
                        android:layout_width="match_parent"
                        android:layout_height="wrap_content"
                        android:layout_below="@id/selectCardGroup"
                        android:layout_marginBottom="10dp"
                        android:layout_marginLeft="16dp"
                        android:layout_marginRight="16dp"
                        android:layout_marginTop="5dp"
                        android:background="@drawable/btn_sign_in_background"
                        android:text="REQUEST"
                        android:textColor="@android:color/white" />

                    <LinearLayout
                        android:id="@+id/selectCardGroup"
                        android:layout_width="wrap_content"
                        android:layout_height="wrap_content"
                        android:layout_centerHorizontal="false"
                        android:layout_marginBottom="1dp"
                        android:layout_marginTop="1dp"
                        android:orientation="vertical">

                        <LinearLayout
                            android:id="@+id/selectCardGroup1"
                            android:layout_width="wrap_content"
                            android:layout_height="wrap_content"
                            android:layout_centerHorizontal="false"
                            android:layout_marginBottom="4dp"
                            android:layout_marginTop="4dp"
                            android:orientation="horizontal">

                            <ImageView
                                android:id="@+id/selectedUberX"
                                android:layout_width="60dp"
                                android:layout_height="40dp"
                                android:layout_marginLeft="8dp"
                                android:src="@drawable/car_cui_select3" />

                            <TextView
                                android:id="@+id/text_view_id1"
                                android:layout_width="wrap_content"
                                android:layout_height="40dp"
                                android:layout_marginLeft="5dp"
                                android:clickable="true"
                                android:gravity="center"
                                android:onClick="vikacar"
                                android:text="Vika Car request pickup" />
                        </LinearLayout>

                        <LinearLayout
                            android:id="@+id/selectCardGroup2"
                            android:layout_width="wrap_content"
                            android:layout_height="wrap_content"
                            android:layout_centerHorizontal="false"
                            android:layout_marginBottom="4dp"
                            android:layout_marginTop="4dp"
                            android:orientation="horizontal">

                            <ImageView
                                android:id="@+id/selectedUberBlack"
                                android:layout_width="60dp"
                                android:layout_height="match_parent"
                                android:layout_marginLeft="8dp"
                                android:src="@drawable/car_vip_select3" />

                            <TextView
                                android:id="@+id/text_view_id2"
                                android:layout_width="wrap_content"
                                android:layout_height="40dp"
                                android:layout_marginLeft="5dp"
                                android:clickable="true"
                                android:gravity="center"
                                android:onClick="vikamoto"
                                android:text="Vika Motorbike request pickup" />

                        </LinearLayout>

                        <LinearLayout
                            android:id="@+id/selectCardGroup3"
                            android:layout_width="wrap_content"
                            android:layout_height="wrap_content"
                            android:layout_centerHorizontal="false"
                            android:layout_marginBottom="4dp"
                            android:layout_marginTop="4dp"
                            android:orientation="horizontal">

                            <ImageView
                                android:id="@+id/selectedUberX1"
                                android:layout_width="60dp"
                                android:layout_height="40dp"
                                android:layout_marginLeft="8dp"
                                android:src="@drawable/tuk" />

                            <TextView
                                android:id="@+id/text_view_id3"
                                android:layout_width="wrap_content"
                                android:layout_height="40dp"
                                android:layout_marginLeft="5dp"
                                android:clickable="true"
                                android:gravity="center"
                                android:onClick="vikatuk"
                                android:text="Vika tuk tuk request pickup" />

                        </LinearLayout>

                    </LinearLayout>

                </RelativeLayout>
            </androidx.cardview.widget.CardView>





 */