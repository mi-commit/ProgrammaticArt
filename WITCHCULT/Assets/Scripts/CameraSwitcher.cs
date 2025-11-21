using UnityEngine;
using System.Collections.Generic;
using System.Collections;
public class CameraSwitcher : MonoBehaviour
{
    static List<CameraSwitcher> switchers = new();
    public Camera cam => GetComponent<Camera>();

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    static public void Switch(float duration)
    {
        foreach(CameraSwitcher c in switchers)
        {
            c.cam.enabled = !c.cam.enabled;
            c.StartCoroutine(c.reSwitchSequence(duration));
        }
    }

     public IEnumerator reSwitchSequence(float time)
    {
        yield return new WaitForSeconds(time);
        cam.enabled = !cam.enabled;
    }

    void Start()
    {
        if (switchers.Contains(this)) Debug.LogError("DUPLICATE CAMERASWITCHER");
        switchers.Add(this);
    }
}
