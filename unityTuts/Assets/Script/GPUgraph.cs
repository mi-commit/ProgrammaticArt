using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UIElements;

public class GPUgraph : MonoBehaviour
{
    //for the gpu to have as references to cube object
    [SerializeField]
    Material material;
    [SerializeField]
    Mesh mesh;


    ComputeBuffer positionBuffer; // reference to a cpu data buffer, not yet initialized
    [SerializeField]
    ComputeShader cShader;

    //used to set properties for compute shader
    static readonly int positionsId = Shader.PropertyToID("_Positions"),    //starting cube positions id, for the buffer
                        resolutionId = Shader.PropertyToID("_Resolution"),
                        transitionProgressId = Shader.PropertyToID("_TransitionProgress"),
		                stepId = Shader.PropertyToID("_Step"), // step between cubes
		                timeId = Shader.PropertyToID("_Time");
        

    const int max_resolution = 1000;
    //points per side
    [SerializeField, Range (5, max_resolution)]
    int resolution;

    [SerializeField]
    FunctionLibrary.FunctionName function;
    [SerializeField]
    FunctionLibrary.FunctionName transFunction;

    //NOT CURRENTLY USED, from Graph.cs, eventually these will also be replaced to work on gpu
    [SerializeField, Min(0f)]
    float functionDuration = 1f,  transitionDuration = 1f;

    float currDuration;

    public bool transitioning;

    //END OF UNUSED



     void OnEnable()
    {
        //count is the total amount of cubes, "stride" or the size of each element is 3 3D positions, each taking 4 bytes, so 3*4
        //taking a constant 11.4 miB of gpu memory 
        positionBuffer = new ComputeBuffer(max_resolution * max_resolution, 3*4);
    }
    private void Update()
    {
        currDuration += Time.deltaTime;
        if (transitioning)
        {
            if (currDuration >= transitionDuration)
            {
                currDuration -= transitionDuration;
                transitioning = false;
            }
        }
        else if (currDuration >= functionDuration)
        {
            currDuration -= functionDuration;
            transitioning = true;
            transFunction = function;
            function = FunctionLibrary.GetRandomFunctionNameOtherThan(function);
        }

        UpdateFuncGPU();
    }
    private void OnDisable()
    {
        //free gpu memory
        positionBuffer.Release();
        positionBuffer = null; // enables the garbage collector to free this even quicker as the reference is null

    }
    void UpdateFuncGPU()
    {
        var KernelIndex = (int)function + (int)(transitioning ? transFunction : function) * FunctionLibrary.FunctionCount;
        //distance between cubes
        float step = 2f / resolution;

        //give the shader new property values
        cShader.SetInt(resolutionId, resolution);
        cShader.SetFloat(stepId, step);
        cShader.SetFloat (timeId, Time.time);
        cShader.SetBuffer(KernelIndex, positionsId,positionBuffer); //buffer also takes a kernel as argument
        if (transitioning)
        {
            cShader.SetFloat(transitionProgressId,
            Mathf.SmoothStep(0, 1, currDuration / functionDuration));
        }



        //function kernel takes fixed groups of 8 * 8, so we need to dispatch x/8 * y/8 times
        int groups = Mathf.CeilToInt(resolution / 8f);
        cShader.Dispatch(KernelIndex, groups, groups, 1); // dispatch to calcucate positions

        //give material access to the now compute shadered values to draw from
        material.SetBuffer(positionsId, positionBuffer); 
        material.SetFloat(stepId, step);

        //boundingBox is needed for culling
        var bounds = new Bounds(Vector3.zero, Vector3.one * (2.0f + 2.0f / resolution)); //based on points remaining within a cube of size 2, with some potentially reaching outside it by half a sizes 

        //draw call
        Graphics.DrawMeshInstancedProcedural(mesh, 0, material,bounds, resolution*resolution);
    }

}
