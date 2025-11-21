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
    public void LookAt(GameObject target)
    {
        if (LookLock) return;
        foreach (Cultist c in cult)
        {
            c.LookAt(target.transform, 4, 9, false);
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
    IEnumerator CameraCycle (float duration)
    {
        if(LookLock)yield break;
        maxFlareDuration = duration/2;
        LookLock = true;
        FlareEffect = true;
        flare.active = true;
        flareIntensityTarget = 400;
        flareStart = 0;

        flareDuration = duration / 4;
        yield return new WaitForSeconds(duration/2);
        flareStart = 400; flareIntensityTarget = 0; flareDuration = duration/4;
        LookLock = false;
        yield return new WaitForSeconds(duration/2);

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
            int EventId = 1;


            switch (EventId)
            {
                case 0:
                    StartCoroutine(DebrisSequence(10));
                    yield return new WaitForSeconds(10);
                    break;
                 case 1:
                    StartCoroutine(CameraCycle(6));
                    yield return new WaitForSeconds(1);
                    CameraSwitcher.Switch(10);
                    yield return new WaitForSeconds(7);
                    StartCoroutine(CameraCycle(6));
                    break;

            }



            yield return new WaitForSeconds(60);
        }
    }




    private IEnumerator DebrisSequence(float TotalDuration)
    {
        StartCoroutine(CameraCycle(TotalDuration));
        float burstDuration = TotalDuration/2;
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