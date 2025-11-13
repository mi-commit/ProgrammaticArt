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

    public Mesh[] MESHES;
    public Material[] MATERIALS;

    void Awake()
    {
        StartCoroutine(RandomCycle());

        cult = GetComponentsInChildren<Cultist>();
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

    IEnumerator RandomCycle()
    {
        while (true)
        {
            float burstDuration = 5f;
            LookAt(Circle);
            while (burstDuration > 0)
            {
                CreateRandomDebris();
                float random = Random.Range(.1f, .5f);
                burstDuration -= random;
                yield return new WaitForSeconds(random);
            }
            LookAt(Sky);
            yield return new WaitForSeconds(60);
        }

    }


    void CreateRandomDebris()
    {
        new GameObject().AddComponent<Debris>().Init(RandomDebris(), 4, transform.position + new Vector3(Random.Range(-1f, 1f), 0, Random.Range(0f, 1f)));
    }

    DebrisData RandomDebris()
    {
        DebrisData d = ScriptableObject.CreateInstance<DebrisData>();
        d.mesh = MESHES[Random.Range(0, MESHES.Length)];
        d.material = MATERIALS[Random.Range(0, MATERIALS.Length)];
        return d;
    }
}