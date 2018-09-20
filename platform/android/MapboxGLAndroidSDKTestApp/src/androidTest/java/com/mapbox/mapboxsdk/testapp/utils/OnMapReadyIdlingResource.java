package com.mapbox.mapboxsdk.testapp.utils;

import android.app.Activity;
import android.os.Handler;
import android.support.annotation.WorkerThread;
import android.support.test.espresso.IdlingResource;

import com.mapbox.mapboxsdk.maps.MapView;
import com.mapbox.mapboxsdk.maps.MapboxMap;
import com.mapbox.mapboxsdk.maps.OnMapReadyCallback;
import com.mapbox.mapboxsdk.testapp.R;

public class OnMapReadyIdlingResource implements IdlingResource, OnMapReadyCallback {

  private MapboxMap mapboxMap;
  private IdlingResource.ResourceCallback resourceCallback;

  @WorkerThread
  public OnMapReadyIdlingResource(Activity activity) {
    new Handler(activity.getMainLooper()).post(() -> {
      try {
        MapView mapView = (MapView) activity.findViewById(R.id.mapView);
        mapView.getMapAsync(OnMapReadyIdlingResource.this);
      } catch (Exception err) {
        throw new RuntimeException(err);
      }
    });
  }

  @Override
  public String getName() {
    return getClass().getSimpleName();
  }

  @Override
  public boolean isIdleNow() {
    return mapboxMap != null;
  }

  @Override
  public void registerIdleTransitionCallback(ResourceCallback resourceCallback) {
    this.resourceCallback = resourceCallback;
  }

  public MapboxMap getMapboxMap() {
    return mapboxMap;
  }

  @Override
  public void onMapReady(MapboxMap mapboxMap) {
    this.mapboxMap = mapboxMap;
    if (resourceCallback != null) {
      resourceCallback.onTransitionToIdle();
    }
  }
}