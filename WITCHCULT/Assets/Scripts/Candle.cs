using NUnit.Framework;
using UnityEngine;
using System.Collections.Generic;
using System.Collections;

public class Candle : MonoBehaviour
{
    [SerializeField]
    float minFlicker = .8f, maxFlicker = 1.2f;

    float IntensityOriginal = 1432f;
    float Intensity;

    Light light;

    static List<Candle> candles = new();

    private Color colorOrig;
    private Color colorTarget;



    static public void ColorChangeEvent(Color _color, float time = 5)
    {
        foreach (Candle candle in candles)
        {
            candle.StartCoroutine(candle.ColorChangeRoutine(_color, time));
        }
    }
    public IEnumerator ColorChangeRoutine(Color switchColor, float time =5)
    {
        Color originalColor = this.light.color;
        
        light.color = switchColor;
        yield return new WaitForSeconds(time);
        light.color = originalColor;
    }




    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        light = GetComponent<Light>();
        IntensityOriginal = light.intensity;
        Intensity = Random.Range(0, 1) == 1 ? light.intensity : light.intensity * .5f;
        light.intensity=Intensity;
        flickerFrequency += Random.Range(-.3f, .4f);
        candles.Add(this);
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
