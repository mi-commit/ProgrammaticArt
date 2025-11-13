using UnityEngine;

public class Candle : MonoBehaviour
{
    [SerializeField]
    float minFlicker = .8f, maxFlicker = 1.2f;

    float IntensityOriginal = 1432f;
    float Intensity;

    Light light;

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        light = GetComponent<Light>();
        IntensityOriginal = light.intensity;
        Intensity = Random.Range(0, 1) == 1 ? light.intensity : light.intensity * .5f;
        light.intensity=Intensity;
        flickerFrequency += Random.Range(-.3f, .4f);
    }

    private float flickerFrequency = .75f;
    private float timer = 0;
    void Update()
    {

        light.intensity = Mathf.Lerp(light.intensity, Intensity, timer / flickerFrequency);

        timer += Time.deltaTime;

        if (timer > flickerFrequency)
        {
            timer -= flickerFrequency;
            Flicker();
        }
    }

    public void Flicker()
    {
        Intensity = Intensity == IntensityOriginal ? Intensity * Random.Range(minFlicker,maxFlicker) : IntensityOriginal;
    }

}
