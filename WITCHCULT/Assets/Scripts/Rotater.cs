using UnityEngine;

public class Rotater : MonoBehaviour
{
    [SerializeField]
    Vector3 RotationSpeed;
    // Update is called once per frame
    void FixedUpdate()
    {
        Rotate();
    }
    protected void Rotate()
    {
        transform.localRotation *= Quaternion.Euler(RotationSpeed * Time.fixedDeltaTime);

    }
}
