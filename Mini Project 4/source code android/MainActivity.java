package com.example.iotminiproject4;

import android.os.Bundle;
import android.widget.CompoundButton;
import android.widget.TextView;
import android.widget.ToggleButton;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity {

    ToggleButton OnOff;
    TextView servo;
    TextView jarak;

    String nilaiJarak;
    String kondisiServo;
    String kondisiSistem;
    DatabaseReference dbref;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        OnOff = findViewById(R.id.tglBtnOnOff);
        servo = findViewById(R.id.kondisiServo);
        jarak = findViewById(R.id.nilaiJarak);

        dbref = FirebaseDatabase.getInstance().getReference();
        dbref.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                nilaiJarak = dataSnapshot.child("node1/jarak").getValue().toString();
                jarak.setText(nilaiJarak);

                kondisiServo = dataSnapshot.child("node1/kondisiServo").getValue().toString();
                if(kondisiServo.equals("1")){
                    servo.setText("servo bergerak");
                }
                else if(kondisiServo.equals("0")){
                    servo.setText("servo idle");
                }

                kondisiSistem = dataSnapshot.child("node1/statusSistem").getValue().toString();
                if(kondisiSistem.equals("0")){
                    OnOff.setChecked(false);
                }
                else{
                    OnOff.setChecked(true);
                }
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });

        OnOff.setOnCheckedChangeListener(new ToggleButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if(b){
                    DatabaseReference onOffRef = FirebaseDatabase.getInstance().getReference("node1/statusSistem");
                    onOffRef.setValue(1);
                }
                else
                {
                    DatabaseReference onOffRef = FirebaseDatabase.getInstance().getReference("node1/statusSistem");
                    DatabaseReference jarakRef = FirebaseDatabase.getInstance().getReference("node1/jarak");
                    DatabaseReference servoRef = FirebaseDatabase.getInstance().getReference("node1/kondisiServo");
                    onOffRef.setValue(0);
                    jarakRef.setValue(0);
                    servoRef.setValue(0);
                }
            }
        });


    }
}