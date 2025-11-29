using UnityEngine;
using System.Collections;
using System.Collections.Generic;

using UnityEngine.Rendering;
using UnityEngine.Rendering.HighDefinition;
public class Coven : MonoBehaviour
{
    [Header("Members")]
    public Cultist[] cult;

    //objects looked at
    [Header("Look targets for the coven")]
    public GameObject Camera;
    public GameObject Circle;
    public GameObject Sky;

    [Header("Debris Components")]
    public Mesh[] MESHES;
    public Material[] MATERIALS;
    [SerializeField]
    float debrisLifeTime = 4;

    [Header("Postprocessing volume")]
    public Volume m_volume;

    [Header("Sequence controls")]

    [SerializeField]
    float Sequence_duration_base = 120;
    [SerializeField]
    float Sequence_duration_delta = 40;


    //flare functionality, private
    bool FlareEffect = false;
    float flareStart;
    float flareDuration;
    float maxFlareDuration;
    float flareIntensityTarget;
    ScreenSpaceLensFlare flare;


    private bool LookLock;

    void Awake()
    {
        cult = GetComponentsInChildren<Cultist>();

        VolumeProfile profile = m_volume.profile;
        if (!profile.TryGet<ScreenSpaceLensFlare>(out var f))
        {
            f = profile.Add<ScreenSpaceLensFlare>(false);
        }
        flare = f;
        flare.active = FlareEffect;
        StartCoroutine(RandomCycle());

    }
    //* LOOKING AT specific THINGS!**********
    public void LookAt(GameObject target, float turnTime = 4, float remainTime = 9)
    {
        if (LookLock) return;
        foreach (Cultist c in cult)
        {
            c.LookAt(target.transform, turnTime, remainTime, false);
        }
    }
    public void LookAtCamera()
    {
        //StartCoroutine(CameraCycle(5));
        foreach (Cultist c in cult)
        {
            c.LookAt(Camera.transform, 1, 5, true);
        }
    }
    //******************************************



    //FLARES N THINGS **********
    IEnumerator CameraCycle(float duration)
    {
        if (LookLock) yield break;
        maxFlareDuration = duration / 2;
        LookLock = true;
        FlareEffect = true;
        flare.active = true;
        flareIntensityTarget = 400;
        flareStart = 0;

        flareDuration = duration / 4;
        yield return new WaitForSeconds(duration / 2);
        flareStart = 400; flareIntensityTarget = 0; flareDuration = duration / 4;
        LookLock = false;
        yield return new WaitForSeconds(duration / 2);

        FlareEffect = false;
        flare.active = false;
    }
    private void Update()
    {
        if (FlareEffect)
        {
            flareDuration -= Time.deltaTime;
            flare.intensity.value = Mathf.Lerp(flareIntensityTarget, flareStart, flareDuration / maxFlareDuration);
        }
    }

    //******************************************
    public IEnumerator RandomCycle()
    {
        while (true)
        {
            int EventId = 5;

            yield return new WaitForSeconds(1);
            switch (EventId)
            {
                case 0:
                    //DEBRIS
                    StartCoroutine(DebrisSequence(10));
                    yield return new WaitForSeconds(10);
                    break;

                case 1:
                    //CAMSWITCH_BASIC
                    StartCoroutine(CameraCycle(6));
                    yield return new WaitForSeconds(1);
                    CameraSwitcher.Switch(10);
                    yield return new WaitForSeconds(7);
                    StartCoroutine(CameraCycle(6));
                    break;

                case 2:
                    //CAMSWITCH_LOOKAROUND
                    StartCoroutine(CameraCycle(6));
                    yield return new WaitForSeconds(1);
                    CameraSwitcher.Switch(17);
                    yield return new WaitForSeconds(3);
                    LookAt(Sky, 4, 4);
                    yield return new WaitForSeconds(4);
                    LookAt(Circle, 4, 9);
                    yield return new WaitForSeconds(10);
                    StartCoroutine(CameraCycle(6));
                    break;

                //sky lookat crap
                case 3:
                case 4:
                    LookAt(Sky, 4, 4);
                    yield return new WaitForSeconds(4);
                    LookAt(Circle, 4, 9);
                    yield return new WaitForSeconds(20);
                    continue;

                //Color change events
                case 5:
                    Candle.ColorChangeEvent(Color.cyan, 5);
                    break;
                case 6:
                    Candle.ColorChangeEvent(new Color(1f,0f,0.9f), 2);
                    break;
                case 7:
                    //tulee kädet silmien etee takaaa
                    break;
                case 8:
                    //jumpscare
                    break;
                case 9:
                    //maxwell
                default:
                    Debug.Log("NULL-EVENT");
                    yield return new WaitForSeconds(10);
                    break;
            }



            yield return new WaitForSeconds(Sequence_duration_base + Random.Range(-Sequence_duration_delta, Sequence_duration_delta));
        }
    }




    private IEnumerator DebrisSequence(float TotalDuration)
    {
        StartCoroutine(CameraCycle(TotalDuration));
        float burstDuration = TotalDuration / 2;
        LookAt(Circle);
        while (burstDuration > 0)
        {
            new GameObject().AddComponent<Debris>().Init(RandomDebris(), debrisLifeTime, transform.position + new Vector3(Random.Range(-1f, 1f), 0, Random.Range(0f, 1f)));
            float random = Random.Range(.1f, .5f);
            burstDuration -= random;
            yield return new WaitForSeconds(random);
        }
        LookAt(Sky);
    }

    DebrisData RandomDebris()
    {
        DebrisData d = ScriptableObject.CreateInstance<DebrisData>();
        d.mesh = MESHES[Random.Range(0, MESHES.Length)];
        d.material = MATERIALS[Random.Range(0, MATERIALS.Length)];
        return d;
    }
}