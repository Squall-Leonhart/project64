package emu.project64.settings;

import emu.project64.AndroidDevice;
import emu.project64.R;
import emu.project64.jni.NativeExports;
import emu.project64.jni.SettingsID;
import emu.project64.jni.VideoSettingID;

import android.content.SharedPreferences;
import android.os.Bundle;
import androidx.appcompat.app.ActionBar;
import androidx.preference.PreferenceManager;
import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentActivity;
import androidx.appcompat.widget.Toolbar;
import android.view.MenuItem;

public class SettingsActivity extends AppCompatActivity implements SharedPreferences.OnSharedPreferenceChangeListener
{
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        /*if (NativeExports.SettingsLoadBool(SettingsID.GameRunning_CPU_Running.getValue()) == true)
        {
            NativeExports.ExternalEvent( SystemEvent.SysEvent_ResumeCPU_FromMenu.getValue());
        }*/
        setContentView(R.layout.settings_activity);

        Toolbar toolbar = (Toolbar) findViewById( R.id.toolbar );
        setSupportActionBar( toolbar );
        getSupportActionBar().setDisplayShowTitleEnabled(false);
        toolbar.setTitle(getString(R.string.settings_title));
        if (AndroidDevice.IS_ICE_CREAM_SANDWICH)
        {
            ActionBar actionbar = getSupportActionBar();
            actionbar.setHomeButtonEnabled(true);
            actionbar.setDisplayHomeAsUpEnabled(true);
        }

        SharedPreferences sharedPrefs = PreferenceManager.getDefaultSharedPreferences(this);
        sharedPrefs.edit().clear()
        /*.putInt("touchscreenScale",NativeExports.UISettingsLoadDword(UISettingID.TouchScreen_ButtonScale.getValue()))
        .putString("touchscreenLayout",NativeExports.UISettingsLoadString(UISettingID.TouchScreen_Layout.getValue()))
        .putBoolean("audio_Enabled",NativeExports.SettingsLoadBool(SettingsID.Plugin_EnableAudio.getValue()))*/
        .putBoolean("UserInterface_BasicMode",NativeExports.SettingsLoadBool(SettingsID.UserInterface_BasicMode.toString()))
        .putBoolean("Debugger_Enabled",NativeExports.SettingsLoadBool(SettingsID.Debugger_Enabled.toString()))
        .putBoolean("Debugger_RecordRecompilerAsm",NativeExports.SettingsLoadBool(SettingsID.Debugger_RecordRecompilerAsm.toString()))
        .putBoolean("Debugger_LimitFPS",NativeExports.SettingsLoadBool(SettingsID.GameRunning_LimitFPS.toString()))
        .putBoolean("Debugger_DisplaySpeed",NativeExports.SettingsLoadBool(SettingsID.UserInterface_DisplayFrameRate.toString()))
        .putBoolean("Debugger_CpuUsage",NativeExports.SettingsLoadBool(SettingsID.UserInterface_ShowCPUPer.toString()))
        .putBoolean("Debugger_RecordExecutionTimes",NativeExports.SettingsLoadBool(SettingsID.Debugger_RecordExecutionTimes.toString()))
        .putBoolean("Debugger_DebugLanguage",NativeExports.SettingsLoadBool(SettingsID.Debugger_DebugLanguage.toString()))
        .putString("Debugger_DisplaySpeedType",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.UserInterface_FrameDisplayType.toString())))
        .putString("Debugger_TraceMD5",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TraceMD5.toString())))
        .putString("Debugger_TraceThread",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TraceThread.toString())))
        .putString("Debugger_TracePath",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TracePath.toString())))
        .putString("Debugger_TraceSettings",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TraceSettings.toString())))
        .putString("Debugger_TraceUnknown",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TraceUnknown.toString())))
        .putString("Debugger_TraceAppInit",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TraceAppInit.toString())))
        .putString("Debugger_TraceAppCleanup",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TraceAppCleanup.toString())))
        .putString("Debugger_TraceN64System",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TraceN64System.toString())))
        .putString("Debugger_TracePlugins",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TracePlugins.toString())))
        .putString("Debugger_TraceGFXPlugin",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TraceGFXPlugin.toString())))
        .putString("Debugger_TraceAudioPlugin",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TraceAudioPlugin.toString())))
        .putString("Debugger_TraceControllerPlugin",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TraceControllerPlugin.toString())))
        .putString("Debugger_TraceRSPPlugin",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TraceRSPPlugin.toString())))
        .putString("Debugger_TraceRSP",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TraceRSP.toString())))
        .putString("Debugger_TraceAudio",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TraceAudio.toString())))
        .putString("Debugger_TraceRegisterCache",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TraceRegisterCache.toString())))
        .putString("Debugger_TraceRecompiler",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TraceRecompiler.toString())))
        .putString("Debugger_TraceTLB",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TraceTLB.toString())))
        .putString("Debugger_TraceProtectedMEM",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TraceProtectedMEM.toString())))
        .putString("Debugger_TraceUserInterface",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TraceUserInterface.toString())))
        .putString("Debugger_TraceRomList",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TraceRomList.toString())))
        .putString("Debugger_TraceExceptionHandler",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.Debugger_TraceExceptionHandler.toString())))
        .putString("Debugger_TraceVideoMD5",String.valueOf(NativeExports.SettingsLoadDword(VideoSettingID.Set_Logging_MD5.toString())))
        .putString("Debugger_TraceVideoThread",String.valueOf(NativeExports.SettingsLoadDword( VideoSettingID.Set_Logging_Thread.toString())))
        .putString("Debugger_TraceVideoPath",String.valueOf(NativeExports.SettingsLoadDword( VideoSettingID.Set_Logging_Path.toString())))
        .putString("Debugger_TraceVideoSettings",String.valueOf(NativeExports.SettingsLoadDword( VideoSettingID.Set_Logging_Settings.toString())))
        .putString("Debugger_TraceVideoUnknown",String.valueOf(NativeExports.SettingsLoadDword( VideoSettingID.Set_Logging_Unknown.toString())))
        .putString("Debugger_TraceVideoGlide64",String.valueOf(NativeExports.SettingsLoadDword(VideoSettingID.Set_Logging_Glide64.toString())))
        .putString("Debugger_TraceVideoInterface",String.valueOf(NativeExports.SettingsLoadDword(VideoSettingID.Set_Logging_Interface.toString())))
        .putString("Debugger_TraceVideoResolution",String.valueOf(NativeExports.SettingsLoadDword(VideoSettingID.Set_Logging_Resolution.toString())))
        .putString("Debugger_TraceVideoGlitch",String.valueOf(NativeExports.SettingsLoadDword( VideoSettingID.Set_Logging_Glitch.toString())))
        .putString("Debugger_TraceTraceVideoRDP",String.valueOf(NativeExports.SettingsLoadDword(VideoSettingID.Set_Logging_VideoRDP.toString())))
        .putString("Debugger_TraceVideoTLUT",String.valueOf(NativeExports.SettingsLoadDword( VideoSettingID.Set_Logging_TLUT.toString())))
        .putString("Debugger_TraceVideoPNG",String.valueOf(NativeExports.SettingsLoadDword(VideoSettingID.Set_Logging_PNG.toString())))
        .putString("Debugger_TraceVideoOGLWrapper",String.valueOf(NativeExports.SettingsLoadDword(VideoSettingID.Set_Logging_OGLWrapper.toString())))
        /*.putString("Debugger_TraceAudioInitShutdown",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.FirstAudioSettings.getValue() + AudioSettingID.Logging_LogAudioInitShutdown.getValue())))
        .putString("Debugger_TraceAudioAudioInterface",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.FirstAudioSettings.getValue() + AudioSettingID.Logging_LogAudioInterface.getValue())))
        .putString("video_screenResolution",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.FirstGfxSettings.getValue() + VideoSettingID.Set_Resolution.getValue())))
        .putString("video_AspectRatio",String.valueOf(NativeExports.SettingsLoadDword(SettingsID.FirstGfxSettings.getValue() + VideoSettingID.Set_aspect.getValue())))*/
        //.putInt("MaxRomsRemembered",NativeExports.UISettingsLoadDword(UISettingID.File_RecentGameFileCount.getValue()))
        .apply();

        sharedPrefs.registerOnSharedPreferenceChangeListener(this);

        if (savedInstanceState == null)
        {
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_placeholder, new SettingsFragment()).commit();
        }
    }

    @Override
    protected void onStop()
    {
        super.onStop();
        SharedPreferences sharedPrefs = PreferenceManager.getDefaultSharedPreferences(this);
        sharedPrefs.unregisterOnSharedPreferenceChangeListener(this);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
        switch (item.getItemId())
        {
        case android.R.id.home:
            if (!getSupportFragmentManager().popBackStackImmediate())
                finish();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key)
    {
        if (key.equals("UserInterface_BasicMode"))
        {
            NativeExports.SettingsSaveBool(SettingsID.UserInterface_BasicMode.toString(), sharedPreferences.getBoolean(key,false));
            getSupportFragmentManager().popBackStackImmediate();
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_placeholder, new SettingsFragment()).commit();
        }
        /*else if (key.equals("Debugger_DebugLanguage"))
        {
            NativeExports.SettingsSaveBool(SettingsID.Debugger_DebugLanguage.getValue(), sharedPreferences.getBoolean(key,false));
            GalleryActivity.LanguageChanged();
        }
        else if (key.equals("touchscreenScale")) { NativeExports.UISettingsSaveDword(UISettingID.TouchScreen_ButtonScale.getValue(), sharedPreferences.getInt(key, 100)); }
        else if (key.equals("touchscreenLayout")) { NativeExports.UISettingsSaveString(UISettingID.TouchScreen_Layout.getValue(), sharedPreferences.getString(key, "Analog")); }
        else if (key.equals("audio_Enabled")) { NativeExports.SettingsSaveBool(SettingsID.Plugin_EnableAudio.getValue(), sharedPreferences.getBoolean(key,false)); }*/
        else if (key.equals("Debugger_Enabled")) { NativeExports.SettingsSaveBool(SettingsID.Debugger_Enabled.toString(), sharedPreferences.getBoolean(key,false)); }
        else if (key.equals("Debugger_RecordRecompilerAsm")) { NativeExports.SettingsSaveBool(SettingsID.Debugger_RecordRecompilerAsm.toString(), sharedPreferences.getBoolean(key,false)); }
        else if (key.equals("Debugger_CpuUsage")) { NativeExports.SettingsSaveBool(SettingsID.UserInterface_ShowCPUPer.toString(), sharedPreferences.getBoolean(key,false)); }
        else if (key.equals("Debugger_RecordExecutionTimes")) { NativeExports.SettingsSaveBool(SettingsID.Debugger_RecordExecutionTimes.toString(), sharedPreferences.getBoolean(key,false)); }
        else if (key.equals("Debugger_LimitFPS")) { NativeExports.SettingsSaveBool(SettingsID.GameRunning_LimitFPS.toString(), sharedPreferences.getBoolean(key,false)); }
        else if (key.equals("Debugger_DisplaySpeed")) { NativeExports.SettingsSaveBool(SettingsID.UserInterface_DisplayFrameRate.toString(), sharedPreferences.getBoolean(key,false)); }
        else if (key.equals("Debugger_DisplaySpeedType")) { NativeExports.SettingsSaveDword(SettingsID.UserInterface_FrameDisplayType.toString(), Integer.valueOf(sharedPreferences.getString(key, "0"))); }
        else if (key.equals("Debugger_TraceMD5")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TraceMD5.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceThread")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TraceThread.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TracePath")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TracePath.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceSettings")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TraceSettings.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceUnknown")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TraceUnknown.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceAppInit")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TraceAppInit.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceAppCleanup")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TraceAppCleanup.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceN64System")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TraceN64System.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TracePlugins")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TracePlugins.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceGFXPlugin")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TraceGFXPlugin.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceAudioPlugin")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TraceAudioPlugin.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceControllerPlugin")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TraceControllerPlugin.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceRSPPlugin")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TraceRSPPlugin.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceRSP")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TraceRSP.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceAudio")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TraceAudio.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceRegisterCache")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TraceRegisterCache.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceRecompiler")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TraceRecompiler.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceTLB")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TraceTLB.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceProtectedMEM")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TraceProtectedMEM.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceUserInterface")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TraceUserInterface.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceRomList")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TraceRomList.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceExceptionHandler")) { NativeExports.SettingsSaveDword(SettingsID.Debugger_TraceExceptionHandler.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceVideoMD5")) { NativeExports.SettingsSaveDword(VideoSettingID.Set_Logging_MD5.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceVideoThread")) { NativeExports.SettingsSaveDword(VideoSettingID.Set_Logging_Thread.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceVideoPath")) { NativeExports.SettingsSaveDword(VideoSettingID.Set_Logging_Path.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceVideoSettings")) { NativeExports.SettingsSaveDword(VideoSettingID.Set_Logging_Settings.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceVideoUnknown")) { NativeExports.SettingsSaveDword(VideoSettingID.Set_Logging_Unknown.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceVideoGlide64")) { NativeExports.SettingsSaveDword(VideoSettingID.Set_Logging_Glide64.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceVideoInterface")) { NativeExports.SettingsSaveDword(VideoSettingID.Set_Logging_Interface.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceVideoResolution")) { NativeExports.SettingsSaveDword(VideoSettingID.Set_Logging_Resolution.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceVideoGlitch")) { NativeExports.SettingsSaveDword(VideoSettingID.Set_Logging_Glitch.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceTraceVideoRDP")) { NativeExports.SettingsSaveDword( VideoSettingID.Set_Logging_VideoRDP.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceVideoTLUT")) { NativeExports.SettingsSaveDword(VideoSettingID.Set_Logging_TLUT.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceVideoPNG")) { NativeExports.SettingsSaveDword(VideoSettingID.Set_Logging_PNG.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceVideoOGLWrapper")) { NativeExports.SettingsSaveDword(VideoSettingID.Set_Logging_OGLWrapper.toString(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        /*else if (key.equals("Debugger_TraceAudioInitShutdown")) { NativeExports.SettingsSaveDword(SettingsID.FirstAudioSettings.getValue() + AudioSettingID.Logging_LogAudioInitShutdown.getValue(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("Debugger_TraceAudioAudioInterface")) { NativeExports.SettingsSaveDword(SettingsID.FirstAudioSettings.getValue() + AudioSettingID.Logging_LogAudioInterface.getValue(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        /*else if (key.equals("video_screenResolution")) { NativeExports.SettingsSaveDword(SettingsID.FirstGfxSettings.getValue() + VideoSettingID.Set_Resolution.getValue(), Integer.valueOf(sharedPreferences.getString(key, "1"))); }
        else if (key.equals("video_AspectRatio"))
        {
            NativeExports.SettingsSaveDword(SettingsID.FirstGfxSettings.getValue() + VideoSettingID.Set_aspect.getValue(), Integer.valueOf(sharedPreferences.getString(key, "0")));
            getSupportFragmentManager().popBackStackImmediate();
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_placeholder, new VideoFragment()).addToBackStack("main").commit();
        }
        else if (key.equals("MaxRomsRemembered")) { NativeExports.UISettingsSaveDword(UISettingID.File_RecentGameFileCount.getValue(), sharedPreferences.getInt(key, 10)); }*/
    }
}
