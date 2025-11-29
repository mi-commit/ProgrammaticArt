using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class MovementListener : MonoBehaviour
{
    [SerializeField]
    float MinTime = 60;
    float LastMessageTime;
    void OnMessageArrived(string msg)
    {
        if(Time.time > LastMessageTime + MinTime)
        {
            LastMessageTime = Time.time;
            Debug.Log(msg);
            GetComponent<Coven>().LookAtCamera();
        }

    }


    void OnConnectionEvent(bool success)
    {
        LastMessageTime = Time.time-MinTime;
        Debug.Log(success ? "Connected" : "Disconnected");
    }
}
