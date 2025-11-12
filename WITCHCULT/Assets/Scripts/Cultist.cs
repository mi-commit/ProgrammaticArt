using UnityEngine;
using System.Collections.Generic;
using UnityEditor;

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
    }

    float TickTime = .1f;
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

    float MovementDuration = 1;

    float progress = 0;
    public void Tick(float delta_time)
    {
        progress += delta_time;
        transform.localRotation = Quaternion.SlerpUnclamped(transform.localRotation, TargetRotation, progress / MovementDuration);
    }
    public void SetTargetRotation(Quaternion _TargetRoation, float _MovementDuration)
    {
        TargetRotation = _TargetRoation;
        MovementDuration = _MovementDuration;
        progress = 0;
    }

    public Bodypart()
    {
        Debug.Log("P");

        DefaultRotation = transform.localRotation;
        TargetRotation = transform.localRotation;
    }

}