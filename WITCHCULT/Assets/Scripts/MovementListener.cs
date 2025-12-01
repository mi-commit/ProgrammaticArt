using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class MovementListener : MonoBehaviour
{
    [SerializeField]
    float MinTime = 60;
    float LastMessageTime;
    Coven coven => GetComponent<Coven>();
    void OnMessageArrived(string msg)
    {
        if(Time.time > LastMessageTime + MinTime)
        {
            LastMessageTime = Time.time;
            Debug.Log(msg);
            coven.StopCoroutine(coven.RandomCycle());
            coven.StartCoroutine(coven.RandomCycle());
            coven.LookAtCamera();
        }

    }


    void OnConnectionEvent(bool success)
    {
        LastMessageTime = Time.time-MinTime;
        Debug.Log(success ? "Connected" : "Disconnected");
    }
}
