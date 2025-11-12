using UnityEngine;
using System.Collections.Generic;
using System.Collections;
using UnityEditor;
using UnityEditor.Experimental.GraphView;

public class Cultist : MonoBehaviour
{
    private List<Bodypart> bodyparts = new();

    [SerializeField]
    GameObject Spine, ShoulderL, ShoulderR, Head;


    enum PartIds
    {
        Spine, ShoulderL, ShoulderR, Head
    }
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        bodyparts.Add(Spine.AddComponent<Bodypart>());
        bodyparts.Add(ShoulderL.AddComponent<Bodypart>());
        bodyparts.Add(Head.AddComponent<Bodypart>());
        bodyparts.Add(ShoulderR.AddComponent<Bodypart>());

        bodyparts[(int)PartIds.Spine].SetTargetRotation(Quaternion.identity, 2);
        foreach(var part in bodyparts)
        {
            part.StartCoroutine(part.RandomCycle(new Vector3(10,10,10)));
        }

    }

    float TickTime = .05f;
    float time = 0;
    private void FixedUpdate()
    {
        time += Time.deltaTime;
        if (time > TickTime)
        {
            time -= TickTime;
            foreach (Bodypart p in bodyparts)
            {

                p.Tick(Time.fixedDeltaTime);
            }
        }

    }
}



public class Bodypart : MonoBehaviour
{
    public Quaternion DefaultRotation { get; private set; }
    Quaternion TargetRotation;

    public float MovementDuration { get; private set; }
    public float progress { get; private set; }

    public void Tick(float delta_time)
    {
        progress += delta_time;
        if (progress >= MovementDuration) return;
        transform.localRotation = Quaternion.SlerpUnclamped(transform.localRotation, TargetRotation, progress / MovementDuration);
    }
    public void SetTargetRotation(Quaternion _TargetRoation, float _MovementDuration)
    {
        TargetRotation = _TargetRoation;
        MovementDuration = _MovementDuration;
        progress = 0;
    }

    public void Awake()
    {
        Debug.Log("Awake");
        DefaultRotation = transform.localRotation;
        TargetRotation = transform.localRotation;
    }

    public IEnumerator RandomCycle(Vector3 Range)
    {
        while (true)
        {
            yield return new WaitForSeconds(MovementDuration - progress + Random.Range(1f,3f));
            Vector3 v = new Vector3(Random.Range(-Range.x,Range.x), Random.Range(-Range.y, Range.y), Random.Range(Range.z, Range.z));
            SetTargetRotation(Quaternion.Euler(DefaultRotation.eulerAngles + v), Random.Range(1f,5f));
        }
    }
}