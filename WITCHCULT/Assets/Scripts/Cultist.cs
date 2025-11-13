using System.Collections;
using System.Collections.Generic;
using UnityEngine;

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
        bodyparts.Add(ShoulderR.AddComponent<Bodypart>());
        bodyparts.Add(Head.AddComponent<Bodypart>());

        foreach (var part in bodyparts)
        {
            part.StartCoroutine(part.RandomCycle(new Vector3(10, 10, 10)));
        }
    }
    public void LookAt(Transform location, float turnTime = 1, float RemainTime =5, bool stopMovement = false)
    {
        if (stopMovement)
        {
            foreach (var part in bodyparts)
            {
                part.StopAllCoroutines();
                part.SetTargetRotation(part.DefaultRotation, turnTime);
                part.StartCoroutine(part.RandomCycle(new Vector3(10, 10, 10), RemainTime));
            }
        }
        else
        {
            bodyparts[(int)PartIds.Head].StopAllCoroutines();
            bodyparts[(int)PartIds.Head].StartCoroutine(bodyparts[(int)PartIds.Head].RandomCycle(new Vector3(10, 10, 10), RemainTime));

        }
        bodyparts[(int)PartIds.Head].LookTowards(location.position, turnTime);
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
        transform.rotation = Quaternion.SlerpUnclamped(transform.rotation, TargetRotation, progress / MovementDuration);
    }
    public void SetTargetRotation(Quaternion _TargetRoation, float _MovementDuration)
    {
        TargetRotation = _TargetRoation;
        MovementDuration = _MovementDuration;
        progress = 0;
    }
    public void LookTowards(Vector3 position, float time)
    {
        SetTargetRotation(Quaternion.LookRotation(position - transform.position), time);
    }

    public void Awake()
    {
        DefaultRotation = transform.rotation;
        TargetRotation = transform.rotation;
    }

    public IEnumerator RandomCycle(Vector3 Range, float Wait = 1)
    {
        while (true)
        {
            yield return new WaitForSeconds(Wait);
            Vector3 v = new Vector3(Random.Range(-Range.x, Range.x), Random.Range(-Range.y, Range.y), Random.Range(Range.z, Range.z));
            SetTargetRotation(Quaternion.Euler(DefaultRotation.eulerAngles + v), Random.Range(1f, 5f));
            Wait = MovementDuration - progress + Random.Range(1f, 3f);
        }
    }
}