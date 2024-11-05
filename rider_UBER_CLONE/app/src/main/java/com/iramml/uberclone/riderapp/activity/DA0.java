package com.iramml.uberclone.riderapp.activity;

import android.content.Intent;
import android.content.IntentFilter;
import android.os.Build;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.google.android.gms.auth.api.phone.SmsRetriever;
import com.google.android.gms.auth.api.phone.SmsRetrieverClient;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.gms.tasks.Task;
import com.google.android.material.snackbar.Snackbar;
import com.google.firebase.FirebaseException;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.PhoneAuthCredential;
import com.google.firebase.auth.PhoneAuthOptions;
import com.google.firebase.auth.PhoneAuthProvider;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;
import com.iramml.uberclone.riderapp.R;
import com.iramml.uberclone.riderapp.common.Common;
import com.iramml.uberclone.riderapp.model.firebase.User;
import com.rengwuxian.materialedittext.MaterialEditText;

import java.util.concurrent.TimeUnit;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;
import dmax.dialog.SpotsDialog;

import com.rilixtech.widget.countrycodepicker.Country;
import com.rilixtech.widget.countrycodepicker.CountryCodePicker;


public class DA0 extends AppCompatActivity {
        private FirebaseAuth mAuth;
        SmsBroadcastReceiver smsBroadcastReceiver;
        public android.app.AlertDialog waitingDialog;
        CountryCodePicker ccp;
        DatabaseReference users;
        FirebaseDatabase firebaseDatabase;
        PhoneAuthCredential credential;
        String phone;
        String fullnumber;
        private MaterialEditText edtPhone, edtOTP;
        private PhoneAuthProvider.OnVerificationStateChangedCallbacks mCallBack;
        private Button verifyOTPBtn, generateOTPBtn;
        private String verificationId;
    ConstraintLayout root;
    AppCompatActivity activity;
        @Override
   protected void onCreate(Bundle savedInstanceState) {
   super.onCreate(savedInstanceState);
   setContentView(R.layout.doa);
   mAuth = FirebaseAuth.getInstance();
   firebaseDatabase=FirebaseDatabase.getInstance();
   users=firebaseDatabase.getReference(Common.user_rider_tbl);
   waitingDialog = new SpotsDialog.Builder().setContext(DA0.this).build();
   edtPhone = findViewById(R.id.idEdtPhoneNumber);
   edtOTP = findViewById(R.id.idEdtOtp);
   verifyOTPBtn = findViewById(R.id.idBtnVerify);
   generateOTPBtn = findViewById(R.id.idBtnGetOtp);

            ccp = (CountryCodePicker) findViewById(R.id.ccp);
            ccp.registerPhoneNumberTextView(edtPhone);
  generateOTPBtn.setOnClickListener(new View.OnClickListener() {
  @Override
  public void onClick(View v) {
      waitingDialog.show();
      if (!edtPhone.getText().toString().isEmpty()) {
          phone= edtPhone.getText().toString().trim();
          String country= ccp.getSelectedCountryName();
          if(ccp.isValid())
          {
              generateOTPBtn.setText("Resend Code");
              fullnumber= ccp.getFullNumberWithPlus();
              sendVerificationCode(fullnumber);
          }
          else
          {
              Toast.makeText(DA0.this, "Enter valid number", Toast.LENGTH_SHORT).show();
              Snackbar.make(findViewById(android.R.id.content), "Enter valid number", Snackbar.LENGTH_SHORT).show();
              waitingDialog.dismiss();
              return;
          }
      } else {
          Toast.makeText(DA0.this, "Please enter phone number", Toast.LENGTH_SHORT).show();
          Snackbar.make(findViewById(android.R.id.content), "Please enter phone number", Snackbar.LENGTH_SHORT).show();
          waitingDialog.dismiss();
          return;
      }

  }
  });

  verifyOTPBtn.setOnClickListener(new View.OnClickListener() {
  @Override
  public void onClick(View v) {
      waitingDialog.show();
  if (TextUtils.isEmpty(edtOTP.getText().toString())) {
  Toast.makeText(DA0.this, "Please enter OTP", Toast.LENGTH_SHORT).show();
      waitingDialog.dismiss();
  } else {
  verifyCode(edtOTP.getText().toString());
  }
  }
  });
            StartFirebaseLogin();
            startSmsUserConsent();
  }


    private void startSmsUserConsent() {
        SmsRetrieverClient client = SmsRetriever.getClient(this);
        client.startSmsUserConsent(null).addOnSuccessListener(new OnSuccessListener<Void>() {
            @Override
            public void onSuccess(Void aVoid) {
                //Toast.makeText(getApplicationContext(), "On Success", Toast.LENGTH_LONG).show();
            }
        }).addOnFailureListener(new OnFailureListener() {
            @Override
            public void onFailure(@NonNull Exception e) {
               // Toast.makeText(getApplicationContext(), "On OnFailure", Toast.LENGTH_LONG).show();
            }
        });
    }

    private void signInWithCredential(PhoneAuthCredential credential) {

  mAuth.signInWithCredential(credential).addOnCompleteListener(new OnCompleteListener<AuthResult>() {
  @Override
  public void onComplete(@NonNull Task<AuthResult> task) {
  if (task.isSuccessful()) {
      User user=new User();
      user.setPhone(edtPhone.getText().toString());

      users.child(FirebaseAuth.getInstance().getCurrentUser().getUid())
              .setValue(user)
              .addOnSuccessListener(new OnSuccessListener<Void>() {
                  @Override
                  public void onSuccess(Void aVoid) {
                      //Snackbar.make(root, activity.getResources().getString(R.string.registered), Snackbar.LENGTH_SHORT).show();
                      //Toast.makeText(DA0.this, activity.getResources().getString(R.string.registered), Toast.LENGTH_LONG).show();
                  }
              }).addOnFailureListener(new OnFailureListener() {
                  @Override
                  public void onFailure(@NonNull Exception e) {
                      //Snackbar.make(root, activity.getResources().getString(R.string.failed)+e.getMessage(), Snackbar.LENGTH_SHORT).show();
                      Toast.makeText(DA0.this, e.getMessage(), Toast.LENGTH_LONG).show();
                  }
              });
      waitingDialog.dismiss();
     Intent i = new Intent(DA0.this, HomeActivity.class);
     startActivity(i);
     finish();
   } else {
    Toast.makeText(DA0.this, task.getException().getMessage(), Toast.LENGTH_LONG).show();
  }
  }
   });

     }


    private void signInWithCredential2(PhoneAuthCredential credential) {

        mAuth.signInWithCredential(credential).addOnCompleteListener(new OnCompleteListener<AuthResult>() {
            @Override
            public void onComplete(@NonNull Task<AuthResult> task) {
                if (task.isSuccessful()) {

                    waitingDialog.dismiss();
                    Intent i = new Intent(DA0.this, HomeActivity.class);
                    startActivity(i);
                    finish();
                } else {
                    Toast.makeText(DA0.this, task.getException().getMessage(), Toast.LENGTH_LONG).show();
                }
            }
        });

    }



      private void sendVerificationCode(String number) {
  PhoneAuthOptions options =
   PhoneAuthOptions.newBuilder(mAuth)
  .setPhoneNumber(number)
  .setTimeout(0L, TimeUnit.SECONDS)
  .setActivity(this)
   .setCallbacks(mCallBack)
   .build();
   PhoneAuthProvider.verifyPhoneNumber(options);
   }
    private void StartFirebaseLogin() {

        mCallBack = new PhoneAuthProvider.OnVerificationStateChangedCallbacks() {


            @Override
            public void onCodeSent(String s, PhoneAuthProvider.ForceResendingToken forceResendingToken) {
                super.onCodeSent(s, forceResendingToken);

                verificationId = s;

            }

            @Override
            public void onVerificationCompleted(PhoneAuthCredential phoneAuthCredential) {

                final String code = phoneAuthCredential.getSmsCode();

                if (code != null) {
                    edtOTP.setText(code);

                    verifyCode(code);
                }
            }

            @Override
            public void onVerificationFailed(FirebaseException e) {
                waitingDialog.dismiss();
                Toast.makeText(DA0.this, e.getMessage(), Toast.LENGTH_LONG).show();
            }
        };
    }

             private void verifyCode (String code){

                 credential = PhoneAuthProvider.getCredential(verificationId, code);
                loginn();
                // signInWithCredential(credential);
             }

    private void registerBroadcastReceiver() {
        smsBroadcastReceiver = new SmsBroadcastReceiver();
        smsBroadcastReceiver.smsBroadcastReceiverListener = new SmsBroadcastReceiver.SmsBroadcastReceiverListener() {
                    @Override
                    public void onSuccess(Intent intent) {
                        waitingDialog.dismiss();
                        startActivityForResult(intent, 200);
                    }
                    @Override
                    public void onFailure() {
                    }
                };
        IntentFilter intentFilter = new IntentFilter(SmsRetriever.SMS_RETRIEVED_ACTION);
        registerReceiver(smsBroadcastReceiver, intentFilter);
    }

    @Override
    protected void onStart() {
        super.onStart();
        registerBroadcastReceiver();
    }
    @Override
    protected void onStop() {
        super.onStop();
        unregisterReceiver(smsBroadcastReceiver);
    }


    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == 200) {
            if ((resultCode == RESULT_OK) && (data != null)) {
                String message = data.getStringExtra(SmsRetriever.EXTRA_SMS_MESSAGE);
                Toast.makeText(getApplicationContext(), message, Toast.LENGTH_LONG).show();
                getOtpFromMessage(message);
            }
        }
    }

    private void getOtpFromMessage(String message) {
        Pattern pattern = Pattern.compile("(|^)\\d{6}");
        Matcher matcher = pattern.matcher(message);
        if (matcher.find()) {
            edtOTP.setText(matcher.group(0));
        }
    }


    private void loginn()
    {
        users.orderByChild("phone")
                .equalTo(edtPhone.getText().toString())
                .addListenerForSingleValueEvent(new ValueEventListener() {
                    @Override
                    public void onDataChange(DataSnapshot dataSnapshot) {
                        if (dataSnapshot.getValue() != null) {

                            signInWithCredential2(credential);
                        }
                        else {
                            signInWithCredential(credential);
                        }

                    }

                    @Override
                    public void onCancelled(DatabaseError databaseError) {

                    }
                });


    }


         }
