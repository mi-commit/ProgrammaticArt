using UnityEngine;
using System.Collections.Generic;
[CreateAssetMenu(fileName = "DebrisData", menuName = "Scriptable Objects/DebrisData")]
public class DebrisData : ScriptableObject
{
    public Mesh mesh;
    public Material material;

    public enum MaterialIds {Rock, Darkness, Glow};
}
