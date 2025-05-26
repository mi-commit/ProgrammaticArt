using UnityEngine;
using UnityEngine.UIElements;
using static UnityEngine.Mathf;
public static class FunctionLibrary
{
    //delegate defines a kind of function to use as a reference
    //so here we define functions that take in x, z, t and return a float
    public delegate Vector3 Function(float u, float v, float t);

    static Function[] functions = { Wave, MultiWave, Ripple, TestBench, Sphere };    //array of functions that we use
    public enum FunctionName { Wave, MultiWave, Ripple, tests, Sphere} //used for convenience in referring to specific functions

    //returns selected function by name
    public static Function GetFunction(FunctionName name)
    {
        if ((int)name < 0 || (int)name >= functions.Length) return Fail;
        return functions[(int)name];
    }

    //returns a sine wave
    public static Vector3 Wave(float u, float v, float t)
    {
        Vector3 p;
        p.x = u; p.z = v;
        p.y= Sin(PI * (u + v + t))/2f; //scaled for visuals
        return p;
    }
    //multiple waves added together
    public static Vector3 MultiWave(float u, float v, float t)
    {
        Vector3 p;
        p.x = u; p.z = v;
        t = t / 2f;
        float y = Sin(PI * (u + t));
        y += Cos(2f * PI * (v + PI*t))*0.5f;
        p.y = 0.5f*y;
        return p;
    }
    //returns a result based on sin of distance to center
    public static Vector3 Ripple(float u, float v, float t)
    {
        Vector3 p;
        p.x = u; p.z = v;

        float d = Sqrt(u * u + v * v);
        float y = Sin(PI * (4f * d - t));
        p.y = y / (1f + 10f * d);
        return p;

    }
    public static Vector3 Sphere(float u, float v, float t)
    {
        float r = Cos(0.5f * PI * v + t + .5f*u);
        Vector3 p = Vector3.zero;
        p.x = r * Sin(PI * u);
        p.y = v;
        p.z = r * Cos(PI * u);

        return p;
    }
    public static Vector3 TestBench(float u, float v, float t)
    {
        Vector3 p;
        p.x = u; p.z = v;

        //return (Lerp(x , z, Abs(Sin(t))) - Lerp(x, z, Abs(Tan(x+z+t))) -Sin(x-z));
        //return (Lerp(x, z, Abs(Sin(t))) - Lerp(x, z, Abs(Tan(x + z + t))));
        //return (Lerp(x, z, Abs(Sin(t))) - Lerp(x, z, Abs(Cos( z -x+ t))));


        //float r = Sqrt(Sin(t)*x * x - Cos(t)*z * z) ;
        //if (0.6 > r && 0.4 < r) p.y = r;
        //p.y = Sin(x + z + t) - 0.5f;

        //if (u == 0) u = 0.0001f;
        //if (v == 0) v = -0.0001f;
        //p.y = -1/((Abs(u) + Abs(v)+ Abs(Cos(t))));

        p.y = Abs(Cos(0.5f*u - Abs(v) + PI * t));
        p.y =  Abs(Cos(Abs(u) - Abs(v) + PI * t));

        p.y = Cos(Abs(u ) - Abs(v) + PI * t);

        //circle with vertical lines doing funky stuff
        //p = Vector3.zero;
        //p.x = Sin(PI * u);
        //p.y = Sin(u) * Sin(v + 1) + Cos(u * t);
        //p.z = Cos(PI * u);

        //"sphre" like thing with a fun type of collapsing rotation and so on. on video
        //float r = Cos(0.5f * PI * v + t + .5f * u);
        //p.x = r * Sin(PI * u);
        //p.y = v;
        //p.z = r * Cos(PI * u);

        return p;
    }
    //if selected invalid input
    public static Vector3 Fail(float u, float v, float t)
    {
        return new Vector3(u, v, 0);
    }
}
