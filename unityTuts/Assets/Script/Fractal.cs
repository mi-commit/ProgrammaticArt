using UnityEngine;

public class Fractal : MonoBehaviour
{
    [SerializeField, Range(1, 8)]
    int depth = 4;

    private void Start()
    {
        name = "fractal " + depth;
        if(depth <= 1)
        {
            return;
        }
        Fractal c1 = CreateChild(Vector3.up, Quaternion.identity);
        Fractal c2 = CreateChild(Vector3.right, Quaternion.Euler(0, 0, -90));
        Fractal c3 = CreateChild(Vector3.left, Quaternion.Euler(0, 0, 90));

        c1.transform.SetParent(transform, false);
        c2.transform.SetParent(transform, false);
        c3.transform.SetParent(transform, false);

    }
    Fractal CreateChild(Vector3 direction, Quaternion rotation)
    {
        Fractal child = Instantiate(this);
        child.depth = depth - 1;
        child.transform.localRotation = rotation;
        child.transform.localPosition = 0.75f * direction;
        child.transform.localScale = 0.5f * Vector3.one;
        return child;
    }
}
