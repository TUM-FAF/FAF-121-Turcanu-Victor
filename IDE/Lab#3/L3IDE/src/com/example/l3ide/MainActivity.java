package com.example.l3ide;

import java.util.ArrayList;

import android.support.v7.app.ActionBarActivity;
import android.support.v7.app.ActionBar;
import android.support.v4.app.Fragment;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnKeyListener;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.os.Build;

public class MainActivity extends ActionBarActivity{
	
    @SuppressWarnings("deprecation")
	@Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
//        if (savedInstanceState == null) {
//            getSupportFragmentManager().beginTransaction()
//                    .add(R.id.container, new PlaceholderFragment())
//                    .commit();
//        }
        final EditText textItem;
    	final Button btnAdd;
    	final Button btnRmv;
    	final ListView listItems;
    	
    	final ArrayList<String> toDoItems;
    	final ArrayAdapter<String> arrAdapt;
    	
    	final MediaPlayer music1 = MediaPlayer.create(this, R.raw.tango1);
    	
    	music1.start();
        
        textItem = (EditText)findViewById(R.id.textItem);
        btnAdd = (Button)findViewById(R.id.btnAdd);
        btnRmv = (Button)findViewById(R.id.btnRmv);
        listItems = (ListView)findViewById(R.id.listItems);
        
        toDoItems = new ArrayList<String>();
        arrAdapt = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, toDoItems);
        listItems.setAdapter(arrAdapt);
        
        btnAdd.setOnClickListener(new OnClickListener(){
			public void onClick(View v) {
				if(textItem.getText().toString().length() > 0){
					toDoItems.add(textItem.getText().toString());
					arrAdapt.notifyDataSetChanged();
					textItem.setText("");
				}
				
			}
        });
        
        btnRmv.setOnClickListener(new OnClickListener(){
			public void onClick(View v) {
				if (!toDoItems.isEmpty()){
					toDoItems.remove(0);
					arrAdapt.notifyDataSetChanged();
				}
			}
		});
        
        Button btnAbout0 = (Button)this.findViewById(R.id.btnAbout);
        
        
        
        final AlertDialog alrtDlg = new AlertDialog.Builder(this).create();
        
        alrtDlg.setTitle("About");
        alrtDlg.setMessage("IDE Lab 3 - Victor Turcanu, FAF-121");
        
        alrtDlg.setButton("Play", new DialogInterface.OnClickListener() {
        	
            public void onClick(DialogInterface dialog, int which) {
            	music1.pause();
            }
        });
        
        alrtDlg.setButton("Stoph", new DialogInterface.OnClickListener() {
			
			@Override
			public void onClick(DialogInterface dialog, int which) {
				
				music1.pause();
			}
		});
        
        btnAbout0.setOnClickListener(new OnClickListener(){
			public void onClick(View v)
			{
				alrtDlg.show();
			}
		});
      
 }
    
    
    
    
    
    

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    /**
     * A placeholder fragment containing a simple view.
     */
    public static class PlaceholderFragment extends Fragment {

        public PlaceholderFragment() {
        }

        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                Bundle savedInstanceState) {
            View rootView = inflater.inflate(R.layout.activity_main, container, false);
            return rootView;
        }
    }


}
