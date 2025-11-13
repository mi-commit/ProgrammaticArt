using UnityEngine;
using System.Collections;
using UnityEngine.Rendering;
using UnityEngine.Rendering.HighDefinition;
public class Coven : MonoBehaviour
{
    public Cultist[] cult;
    
    public GameObject Camera;
    public GameObject Circle;
    public GameObject Sky;

    public Mesh[] MESHES;
    public Material[] MATERIALS;

    public Volume m_volume;
    private bool LookLock;

    bool FlareEffect = false;
    float flareStart;
    float flareDuration;
    float flareIntensityTarget;

    ScreenSpaceLensFlare flare;
    void Awake()
    {
        StartCoroutine(RandomCycle());
        VolumeProfile profile = m_volume.profile;
        if (!profile.TryGet<ScreenSpaceLensFlare>(out var f))
        {
            f = profile.Add<ScreenSpaceLensFlare>(false);
        }
        flare = f;
        flare.active = FlareEffect;
        cult = GetComponentsInChildren<Cultist>();
    }
    public void LookAtCamera()
    {
        StartCoroutine(CameraCycle(5));
        foreach (Cultist c in cult)
        {
            c.LookAt(Camera.transform, 1, 5, true);
        }
    }
    private void Update()
    {
        if (FlareEffect)
        {
            flareDuration -= Time.deltaTime;
            flare.intensity.value = Mathf.Lerp(flareIntensityTarget, 0, flareDuration/5);  
        }
    }
    IEnumerator CameraCycle (float duration)
    {
        if(LookLock)yield return null;
        LookLock = true;
        FlareEffect = true;
        flare.active = true;
        flareIntensityTarget = 400;
        flareStart = 0;
        flareDuration = duration;
        yield return new WaitForSeconds(duration);
        flareStart = 400; flareIntensityTarget = 0; flareDuration = duration;
        yield return new WaitForSeconds(duration);


        flare.active = false;
        LookLock = false;
    }
    public void LookAt(GameObject target)
    {
        if (LookLock) return;
        foreach (Cultist c in cult)
        {
            c.LookAt(target.transform, 4, 9, false);
        }
    }

    public IEnumerator RandomCycle()
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