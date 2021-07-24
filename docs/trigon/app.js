var Vector2D = function (x, y, unitaryVector) {
    this.x = x;
    this.y = y;
    this.unitary = false;

    this.setUnitary = function () {
        var nx = this.x * this.x;
        var ny = this.y * this.y;
        var size = Math.sqrt(nx + ny);

        this.x = this.x / size;
        this.y = this.y / size;
        this.unitary = true;

        return this;
    };

    this.copy = function () {
        return new Vector2D(
            this.x,
            this.y,
            this.unitary
        );
    };

    this.rotate = function (angle) {

        angle = angle * Math.PI / 180;

        var nx = this.x * Math.cos(angle) - this.y * Math.sin(angle);
        var ny = this.x * Math.sin(angle) + this.y * Math.cos(angle);

        this.x = nx;
        this.y = ny;

        return this;
    };

    this.getAngle = function () {
        var angle = Math.atan2(this.y, this.x);
        angle = angle || 0;
        return 180 * (angle / Math.PI);
    };

    this.log = function () {
        console.log({
            x: this.x,
            y: this.y,
            angle: this.getAngle(),
            unitary: this.unitary
        });
    };

    if (unitaryVector) {
        this.setUnitary();
    }
};

var Vector3D = function (x, y, z, unitaryVector) {
    this.x = x;
    this.y = y;
    this.z = z;
    this.unitary = false;

    this.setUnitary = function () {
        var nx = this.x * this.x;
        var ny = this.y * this.y;
        var nz = this.z * this.z;
        var size = Math.sqrt(nx + ny + nz);

        this.x = this.x / size;
        this.y = this.y / size;
        this.z = this.z / size;
        this.unitary = true;

        return this;
    };

    this.copy = function () {
        return new Vector3D(
            this.x,
            this.y,
            this.z,
            this.unitary
        );
    };

    this.getXYProjection = function () {
        var vec2 = new Vector2D(this.x, this.y);
        return vec2;
    };

    this.getXZProjection = function () {
        var vec2 = new Vector2D(this.x, this.z);
        return vec2;
    };

    this.getYZProjection = function () {
        var vec2 = new Vector2D(this.y, this.z);
        return vec2;
    };

    this.rotateX = function (angle) {

        angle = angle * Math.PI / 180;

        var nx = this.x;
        var ny = this.y * Math.cos(angle) - this.z * Math.sin(angle);
        var nz = this.y * Math.sin(angle) + this.z * Math.cos(angle);

        this.x = nx;
        this.y = ny;
        this.z = nz;

        return this;
    };

    this.rotateY = function (angle) {

        angle = angle * Math.PI / 180;

        var nx = this.x * Math.cos(angle) + this.z * Math.sin(angle);
        var ny = this.y;
        var nz = this.x * Math.sin(angle) + this.z * Math.cos(angle);

        this.x = nx;
        this.y = ny;
        this.z = nz;

        return this;
    };

    this.rotateZ = function (angle) {

        angle = angle * Math.PI / 180;

        var nx = this.x * Math.cos(angle) - this.y * Math.sin(angle);
        var ny = this.x * Math.sin(angle) + this.y * Math.cos(angle);
        var nz = this.z;

        this.x = nx;
        this.y = ny;
        this.z = nz;

        return this;
    };

    this.getAlphaAngle = function () {
        var v1 = this.getXYProjection();
        v1.setUnitary();
        return v1.getAngle();
    };

    this.calcBetaOffset = function (v) {
        var beta2 = v.getBetaAngle();
        var beta1 = this.getBetaAngle();

        return beta2 - beta1;
    };

    this.calcAlphaOffset = function (v) {
        var alpha2 = v.getAlphaAngle();
        var alpha1 = this.getAlphaAngle();

        return alpha2 - alpha1;
    };

    this.getBetaAngle = function () {
        var angle = 180 * Math.asin(this.z) / Math.PI;
        return angle + 90;
    };

    this.addAlphaAngle = function (angle) {
        this.rotateZ(angle);

        return this;
    };

    this.addAngles = function (alpha, beta) {
        this.addBetaAngle(beta);
        this.addAlphaAngle(alpha);

        return this;
    };

    this.addBetaAngle = function (angle) {
        var v = this.copy();
        var alphaAngle = this.getAlphaAngle();

        if (!isNaN(alphaAngle)) {
            v.addAlphaAngle(-alphaAngle);
        }

        var projectionXZ = v.getXZProjection().setUnitary();
        projectionXZ.rotate(angle);
        v.x = projectionXZ.x;
        v.y = 0.0;
        v.z = projectionXZ.y;

        if (!isNaN(alphaAngle)) {
            v.addAlphaAngle(alphaAngle);
        }

        this.x = v.x;
        this.y = v.y;
        this.z = v.z;

        return this;
    };

    this.getAngles = function () {
        return {
            alpha: this.getAlphaAngle(),
            beta: this.getBetaAngle()
        };
    };

    this.log = function () {
        var angles = this.getAngles();
        console.log({
            x: this.x,
            y: this.y,
            z: this.z,
            alpha: angles.alpha,
            beta: angles.beta,
            unitary: this.unitary
        });
    };

    if (unitaryVector) {
        this.setUnitary();
    }
};

function getInclination(v_mag, v_accel) {
    var alpha = v_mag.getAlphaAngle();
    console.log(alpha);
    // var accel_copy = v_accel.copy().addAlphaAngle((360 - alpha) % 360);
    var accel_copy = v_accel.copy().addAlphaAngle(-alpha);
    console.log(accel_copy.getAlphaAngle());
    var accel_projected = accel_copy.getXZProjection().setUnitary();
    console.log(accel_projected.getAngle());
    var angle = accel_projected.getAngle() + 90;
    console.log(angle);
    // angle = (360 + angle) % 360;
    // angle = angle % 360;
    console.log(angle);
    if (angle < 180) {
        angle = -angle;
    } else {
        angle = 360 - angle;
    }
    console.log(angle);

    return angle;
}

function setInclination(v_mag, v_accel) {
    var angle = getInclination(v_mag, v_accel);
    v_mag.addBetaAngle(angle);
}

var tests1 = [
    new Vector3D(1, 0, 0),
    new Vector3D(-1, 0, 0),
    new Vector3D(0, 1, 0),
    new Vector3D(0, -1, 0),
    new Vector3D(0, 0, 1),
    new Vector3D(0, 0, -1),
];

var tests2 = [
    new Vector3D(1, 1, 0, true),
    new Vector3D(1, -1, 0, true),
    new Vector3D(-1, 1, 0, true),
    new Vector3D(-1, -1, 0, true),
    new Vector3D(0, 1, 1, true),
    new Vector3D(0, 1, -1, true),
    new Vector3D(0, -1, 1, true),
    new Vector3D(0, -1, -1, true),
    new Vector3D(1, 0, 1, true),
    new Vector3D(1, 0, -1, true),
    new Vector3D(-1, 0, 1, true),
    new Vector3D(-1, 0, -1, true),
];

var tests3 = [
    new Vector3D(1, 1, 1, true),
    new Vector3D(1, 1, -1, true),
    new Vector3D(1, -1, 1, true),
    new Vector3D(1, -1, -1, true),
    new Vector3D(-1, 1, 1, true),
    new Vector3D(-1, 1, -1, true),
    new Vector3D(-1, -1, 1, true),
    new Vector3D(-1, -1, -1, true),
];

tests1.forEach(item => {
    item.log();
});
console.log("----");

tests1.forEach(item => {
    item.addBetaAngle(45);
    item.log();
});
console.log("----");

tests1.forEach(item => {
    item.addBetaAngle(45);
    item.log();
});
console.log("----");

var v1 = new Vector3D(1, 0, 0, true);
// var v2 = new Vector3D(1, 1, 1, true);
// var v2 = new Vector3D(1, 0, 1, true);
var v2 = new Vector3D(0, 1, 1, true);

v1.log();
v2.log();
console.log(v1.calcAlphaOffset(v2));
console.log(v1.calcBetaOffset(v2));
// tests2.forEach(item => {
//     item.log();
// });

// tests3.forEach(item => {
//     item.log();
// });


// var mag = new Vector3D(1, 0, 1, true);
// var accel = new Vector3D(-0.5, 0, 1, true);
console.log("mag x accel");
var mag = new Vector3D(-0.5, 0, -1, true);
var accel = new Vector3D(0.5, 0, -1, true);
mag.log();
accel.log();
console.log(getInclination(mag, accel));

console.log("mag x accel");
mag = new Vector3D(-0.9, 0, -0.438, true);
accel = new Vector3D(-0.438, 0, -0.9, true);
mag.log();
accel.log();

console.log(getInclination(mag, accel));

console.log("mag x accel");
mag = new Vector3D(0.9, 0, -0.438, true);
accel = new Vector3D(-0.438, 0, -0.9, true);
mag.log();
accel.log();

console.log(getInclination(mag, accel));
setInclination(mag, accel);
mag.log();

var TestVectors = {
    isEqual: function (v1, v2) {
        var diffX = Math.abs(v1.x - v2.x);
        var diffY = Math.abs(v1.y - v2.y);
        var diffZ = Math.abs(v1.z - v2.z);
        var diffLimit = 0.01;
        return (diffX < diffLimit && diffY < diffLimit && diffZ < diffLimit);
    },
    createRandomVector: function () {
        var maxRange = 10000;
        var middle = maxRange / 2;
        var vx = Math.random() * maxRange - middle;
        var vy = Math.random() * maxRange - middle;
        var vz = Math.random() * maxRange - middle;
        return new Vector3D(vx, vy, vz, true);
    },
    generateOriginTest: function () {
        var v0 = new Vector3D(0, 0, -1, true);
        var v1 = TestVectors.createRandomVector();

        var v1Alpha = v1.getAlphaAngle();
        var v1Beta = v1.getBetaAngle();
        v1.addAngles(-v1Alpha, -v1Beta);
        return TestVectors.isEqual(v0, v1);
    },
    generateRotateTest: function () {
        var v0 = new Vector3D(0, 0, -1, true);
        var v1 = TestVectors.createRandomVector();

        var v1Alpha = v1.getAlphaAngle();
        var v1Beta = v1.getBetaAngle();

        v0.addAngles(v1Alpha, v1Beta);
        return TestVectors.isEqual(v0, v1);
    },
    logStatus: function (testName, success, fails) {
        var status = 'font-weight:bold;';
        if (fails) {
            status = status + 'color: red;';
        } else {
            status = status + 'color: green;';
        }

        console.log('%c[' + testName + "] Success: " + success + " Fail: " + fails, status);
    },
    runTests: function (test, testName) {
        var maxTests = 10000;
        var testCount = 0;
        var testSuccess = 0;
        var testFail = 0;
        while (testCount < maxTests) {
            testCount++;
            if (test()) {
                testSuccess++;
            } else {
                testFail++;
            }
        }
        TestVectors.logStatus(testName, testSuccess, testFail);
    },
    runAllTests: function () {
        TestVectors.runTests(TestVectors.generateOriginTest, "Origin Test");
        TestVectors.runTests(TestVectors.generateRotateTest, "Rotate Test");
    }
};

TestVectors.runAllTests();