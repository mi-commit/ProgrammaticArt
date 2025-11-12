using UnityEngine;

public class Candle : MonoBehaviour
{

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
        flickerFrequency += Random.Range(-.3f, 1f);
        LightController.candles.Add(this);
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
        Debug.Log("Flick");
        Intensity = Intensity == IntensityOriginal ? Intensity * Random.Range(.8f,1.2f) : IntensityOriginal;
    }

}
