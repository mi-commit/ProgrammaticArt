using UnityEngine;
using System.Collections.Generic;
using System.Collections;
using TMPro;
using static UnityEngine.GraphicsBuffer;
using UnityEditor;
public class Coven : MonoBehaviour
{
    public Cultist[] cult;
    
    public GameObject Camera;
    public GameObject Circle;
    public GameObject Sky;



    void Start()
    {
        cult = GetComponentsInChildren<Cultist>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void LookAtCamera()
    {
        foreach(Cultist c in cult)
        {
            c.LookAt(Camera.transform, 1, 5, true);
        }
    }

    public void LookAt(GameObject target)
    {
        foreach (Cultist c in cult)
        {
            c.LookAt(target.transform, 4, 9, false);
        }
    }
}