
import "./vec3.js";
import { vec3, vec3Set } from "./vec3.js";

async function main() {
    let bar: vec3 = new vec3(0, 0, 0);
    let foo: vec3 = new vec3(1,1,1);

    let crs: vec3 = vec3Set(1, 1, 1);

    console.log("hello!");
}

main()