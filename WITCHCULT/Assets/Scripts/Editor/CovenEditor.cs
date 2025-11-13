using UnityEditor;
using UnityEngine;

[CustomEditor(typeof(Coven))]
class CovenEditor : Editor
{
    public override void OnInspectorGUI()
    {
        DrawDefaultInspector();
        if(EditorApplication.isPlaying && target is Coven c)
        {
            if (GUILayout.Button("Look at Camera") )
            {
                c.LookAtCamera();
            }
            if (GUILayout.Button("Look up"))
            {
                c.LookAt(c.Sky);
            }
            if(GUILayout.Button("Look down"))
            {
                c.LookAt(c.Circle);
            }
            if (GUILayout.Button("restart Debris-sequence"))
            {
                c.StopCoroutine(c.RandomCycle());
                c.StartCoroutine(c.RandomCycle());
            }
        }


    }
}