using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UIElements;

public class GPUgraph : MonoBehaviour
{
    [SerializeField]
    Material material;
    [SerializeField]
    Mesh mesh;

    ComputeBuffer positionBuffer;
    [SerializeField]
    ComputeShader cShader;
    static readonly int positionsId = Shader.PropertyToID("_Positions"),
    resolutionId = Shader.PropertyToID("_Resolution"),
		stepId = Shader.PropertyToID("_Step"),
		timeId = Shader.PropertyToID("_Time");


    //points per side
    [SerializeField, Range (5, 1000)]
    int resolution;

    //select which function to visualize in the inspector
    [SerializeField]
    FunctionLibrary.FunctionName function;

    [SerializeField, Min(0f)]
    float functionDuration = 1f;
    bool transitioning;
    FunctionLibrary.FunctionName transFunction;


    float currDuration;

     void OnEnable()
    {
        positionBuffer = new ComputeBuffer(resolution * resolution, 3*4);
    }
    private void Update()
    {
        UpdateFuncGPU();
    }
    private void OnDisable()
    {
        positionBuffer.Release();
        positionBuffer = null;

    }
    void UpdateFuncGPU()
    {
        float step = 2f / resolution;
        cShader.SetInt(resolutionId, resolution);
        cShader.SetFloat(stepId, step);
        cShader.SetFloat (timeId, Time.time);

        cShader.SetBuffer(0,positionsId,positionBuffer);
        int groups = Mathf.CeilToInt(resolution / 8f);
        cShader.Dispatch(0, groups, groups, 1);

        material.SetBuffer(positionsId, positionBuffer);
        material.SetFloat(stepId, step);
        var bounds = new Bounds(Vector3.zero, Vector3.one* (2.0f + 2.0f / resolution));
        Graphics.DrawMeshInstancedProcedural(mesh, 0, material,bounds, positionBuffer.count);
    }

}
