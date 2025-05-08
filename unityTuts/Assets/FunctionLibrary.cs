using UnityEngine;
using UnityEngine.UIElements;
using static UnityEngine.Mathf;
public static class FunctionLibrary
{
    //delegate defines a kind of function to use as a reference
    //so here we define functions that take in x, z, t and return a float
    public delegate float Function(float x, float z, float t);

    static Function[] functions = { Wave, MultiWave, Ripple, TestBench };    //array of functions that we use
    public enum FunctionName { Wave, MultiWave, Ripple, tests} //used for convenience in referring to specific functions

    //returns selected function by name
    public static Function GetFunction(FunctionName name)
    {
        if ((int)name < 0 || (int)name >= functions.Length) return Fail;
        return functions[(int)name];
    }

    //returns a sine wave
    public static float Wave(float x, float z, float t)
    {
        return Sin(PI * (x + z + t))/2f; //scaled for visuals
    }
    //multiple waves added together
    public static float MultiWave(float x, float z, float t)
    {
        t = t / 2f;
        float y = Sin(PI * (x + t));
        y += Cos(2f * PI * (z + PI*t))*0.5f;
        return 0.5f*y;
    }
    //returns a result based on sin of distance to center
    public static float Ripple(float x,float z, float t)
    {
        float d = Sqrt(x * x + z * z);
        float y = Sin(PI * (4f * d - t));
        return y / (1f + 10f * d);
    }
    public static float TestBench(float x, float z, float t)
    {
        float r = Sqrt(Sin(t)*x * x - Cos(t)*z * z) ;
        if (0.6 > r && 0.4 < r) return r;

        return Sin(x + z + t) - 0.5f;

        //if (x == 0) x = 0.0001f;
        //if (z == 0) z = -0.0001f;
        //return -1/((Abs(x) + Abs(z)+ Abs(Cos(t))));

       //return Cos(Abs(x ) - Abs(z) + PI * t);
       
    }
    //if selected invalid input
    public static float Fail(float x, float z, float t)
    {
        return 0;
    }
}
