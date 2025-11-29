using UnityEngine;

public class TransformRotater : Rotater
{
    [SerializeField]
    private float lifeTime = 5;

    [SerializeField]
    private Vector3 translate;


    private void FixedUpdate()
    {
        lifeTime -= Time.fixedDeltaTime;
        if (lifeTime < 0) Destroy(gameObject);
        Rotate();
        transform.position += translate * Time.fixedDeltaTime;
    }

}
