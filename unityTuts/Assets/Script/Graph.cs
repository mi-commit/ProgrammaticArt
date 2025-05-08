using Unity.VisualScripting;
using UnityEngine;

public class Graph : MonoBehaviour
{
    //select prefab used for visualization
    [SerializeField]
    Transform pointPrefab;

    //points per side
    [SerializeField, Range (5, 500)]
    int resolution;

    //select which function to visualize in the inspector
    [SerializeField]
    FunctionLibrary.FunctionName function;

    //list of points
    Transform[] points;

    private void Awake()
    {
        Vector3 position =Vector3.zero; 

        float step = 2f / resolution; //used for scaling points to fit range [-1,1]
        var scale = Vector3.one * step;

        points = new Transform[resolution * resolution];
        //intresting way of looping through points in a 2d manner
        //ill investigate other ways to use for loops as this is new to me
        for(int i = 0, x=0, z=0; i< points.Length; i++, x++)
        {
            if (x == resolution){//end of a line
                x = 0;
                z += 1;
            }

            Transform point = points[i] = Instantiate(pointPrefab);

            //math to align points at the centers of their grid position
            position.x = ((x + 0.5f) * step - 1f);
            position.z = ((z + 0.5f) * step - 1f);

            point.localPosition = position;
            point.localScale = scale;

            point.SetParent(transform, false);
        }
    }
    private void Update()
    {
        //get the function to visualize
        //i think this is why one should consider what they name things slightly further in advance
        //keeping this here as its funny, and inconsequential as this is a learning test project
        //actually made it even worse because funny
        FunctionLibrary.Function Function = FunctionLibrary.GetFunction(function);

        float time = Time.time;

        for (int i = 0; i < points.Length; i++)
        {
            Transform point = points[i];
            Vector3 position = point.localPosition;
            position.y = Function(position.x,position.z, time);
            point.localPosition = position;
        }
    }
}
