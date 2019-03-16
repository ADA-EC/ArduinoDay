package br.usp.icmc.esp8266mqtt;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

public class MainActivity extends AppCompatActivity {

    private EditText mensagem;
    private Button botao;
    private final String USERNAME = "zwrbsgco";
    private final String PASSWORD = "Gu19Jq6j-ruy";
    private String topic = "ADA/SEnC";
    private byte[] texto = "teste".getBytes();
    private MqttAndroidClient client;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //Conexão ao broker
        String clientId = MqttClient.generateClientId();
        client =
                new MqttAndroidClient(this.getApplicationContext(), "tcp://m11.cloudmqtt.com:18002",
                        clientId);

        MqttConnectOptions options = new MqttConnectOptions();
        options.setMqttVersion(MqttConnectOptions.MQTT_VERSION_3_1);
        options.setUserName(USERNAME);
        options.setPassword(PASSWORD.toCharArray());

        try {
            IMqttToken token = client.connect(options);
            if(!client.isConnected()){
                Log.i("ERROR", "Deu ruim no cliente carai");
            }
            token.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    // We are connected
                    Log.d("TAG", "onSuccess");

                    mensagem = findViewById(R.id.textoMensagem);
                    botao = findViewById(R.id.botaoMensagem);

                    //num = new Random();

                    botao.setOnClickListener(new View.OnClickListener() {
                         @Override
                         public void onClick(View v) {
                             texto = mensagem.getText().toString().getBytes();

                             if(!texto.toString().isEmpty()){
                                 try {
                                     //encodedPayload = texto.getBytes("UTF-8");
                                     MqttMessage message = new MqttMessage(texto);
                                     client.publish(topic, message);
                                 } catch (MqttException e) {
                                     e.printStackTrace();
                                 }

                                 Toast.makeText(MainActivity.this, "Mensagem enviada com sucesso!", Toast.LENGTH_SHORT).show();

                             }
                             else{
                                 Toast.makeText(MainActivity.this, "Preencha algum texto.", Toast.LENGTH_SHORT).show();
                             }

                    /*
                    num.nextInt(3);
                    if(num.equals(0)){
                        textoApresentacao = "Escreve aí rapá!";
                    }
                    else if(num.equals(1)){
                        textoApresentacao = "Não tenha medo!";
                    }
                    else if(num.equals(2)){
                        textoApresentacao = "Use sua criatividade!";
                    }

                    mensagem.setHint(textoApresentacao);
                    */
                                                 }
                                             }
                    );
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    // Something went wrong e.g. connection timeout or firewall problems
                    Log.d("TAG", "onFailure");
                    Toast.makeText(MainActivity.this, "Não foi possível fazer a conexão MQTT.", Toast.LENGTH_SHORT).show();

                    finish();

                }
            });
        } catch (MqttException e) {
            e.printStackTrace();
        }


    }
}
