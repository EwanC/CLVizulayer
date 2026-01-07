// Copyright (c) 2025-2026 Ewan Crawford

// RUN: %build -o %t
// RUN: VIZ_COLOR=1 VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=filled]
// CHECK-NEXT: cluster_0 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_0[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="aliceblue"];
// CHECK-NEXT: }
// CHECK-NEXT: cluster_1 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_1[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="aqua"];
// CHECK-NEXT: }
// CHECK-NEXT: node_0 -> node_1 [label="blocking host wait"
// CHECK-SAME: ltail=cluster_0 lhead=cluster_1];
// CHECK-NEXT: cluster_2 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_2[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="aquamarine"];
// CHECK-NEXT: }
// CHECK-NEXT: node_1 -> node_2 [label="blocking host wait"
// CHECK-SAME: ltail=cluster_1 lhead=cluster_2];
// CHECK-NEXT: cluster_3 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_3[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="azure"];
// CHECK-NEXT: }
// CHECK-NEXT: node_2 -> node_3 [label="blocking host wait"
// CHECK-SAME: ltail=cluster_2 lhead=cluster_3];
// CHECK-NEXT: cluster_4 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_4[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="beige"];
// CHECK-NEXT: }
// CHECK-NEXT: node_3 -> node_4 [label="blocking host wait
// CHECK-SAME: ltail=cluster_3 lhead=cluster_4];
// CHECK-NEXT: cluster_5 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_5[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="bisque"];
// CHECK-NEXT: }
// CHECK-NEXT: node_4 -> node_5 [label="blocking host wait
// CHECK-SAME: ltail=cluster_4 lhead=cluster_5];
// CHECK-NEXT: cluster_6 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_6[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="blue"];
// CHECK-NEXT: }
// CHECK-NEXT: node_5 -> node_6 [label="blocking host wait
// CHECK-SAME: ltail=cluster_5 lhead=cluster_6];
// CHECK-NEXT: cluster_7 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_7[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="blueviolet"];
// CHECK-NEXT: }
// CHECK-NEXT: node_6 -> node_7 [label="blocking host wait
// CHECK-SAME: ltail=cluster_6 lhead=cluster_7];
// CHECK-NEXT: cluster_8 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_8[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="brown"];
// CHECK-NEXT: }
// CHECK-NEXT: node_7 -> node_8 [label="blocking host wait
// CHECK-SAME: ltail=cluster_7 lhead=cluster_8];
// CHECK-NEXT: cluster_9 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_9[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="burlywood"];
// CHECK-NEXT: }
// CHECK-NEXT: node_8 -> node_9 [label="blocking host wait
// CHECK-SAME: ltail=cluster_8 lhead=cluster_9];
// CHECK-NEXT: cluster_10 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_10[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="cadetblue"];
// CHECK-NEXT: }
// CHECK-NEXT: node_9 -> node_10 [label="blocking host wait
// CHECK-SAME: ltail=cluster_9 lhead=cluster_10];
// CHECK-NEXT: cluster_11 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_11[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="chartreuse"];
// CHECK-NEXT: }
// CHECK-NEXT: node_10 -> node_11 [label="blocking host wait
// CHECK-SAME: ltail=cluster_10 lhead=cluster_11];
// CHECK-NEXT: cluster_12 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_12[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="chocolate"];
// CHECK-NEXT: }
// CHECK-NEXT: node_11 -> node_12 [label="blocking host wait
// CHECK-SAME: ltail=cluster_11 lhead=cluster_12];
// CHECK-NEXT: cluster_13 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_13[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="coral"];
// CHECK-NEXT: }
// CHECK-NEXT: node_12 -> node_13 [label="blocking host wait
// CHECK-SAME: ltail=cluster_12 lhead=cluster_13];
// CHECK-NEXT: cluster_14 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_14[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="cornflowerblue"];
// CHECK-NEXT: }
// CHECK-NEXT: node_13 -> node_14 [label="blocking host wait
// CHECK-SAME: ltail=cluster_13 lhead=cluster_14];
// CHECK-NEXT: cluster_15 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_15[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="cornsilk"];
// CHECK-NEXT: }
// CHECK-NEXT: node_14 -> node_15 [label="blocking host wait
// CHECK-SAME: ltail=cluster_14 lhead=cluster_15];
// CHECK-NEXT: cluster_16 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_16[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="crimson"];
// CHECK-NEXT: }
// CHECK-NEXT: node_15 -> node_16 [label="blocking host wait
// CHECK-SAME: ltail=cluster_15 lhead=cluster_16];
// CHECK-NEXT: cluster_17 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_17[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="cyan"];
// CHECK-NEXT: }
// CHECK-NEXT: node_16 -> node_17 [label="blocking host wait
// CHECK-SAME: ltail=cluster_16 lhead=cluster_17];
// CHECK-NEXT: cluster_18 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_18[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="cyan"];
// CHECK-NEXT: }
// CHECK-NEXT: node_17 -> node_18 [label="blocking host wait
// CHECK-SAME: ltail=cluster_17 lhead=cluster_18];
// CHECK-NEXT: cluster_19 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_19[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="darkgoldenrod"];
// CHECK-NEXT: }
// CHECK-NEXT: node_18 -> node_19 [label="blocking host wait
// CHECK-SAME: ltail=cluster_18 lhead=cluster_19];
// CHECK-NEXT: cluster_20 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_20[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="darkgray"];
// CHECK-NEXT: }
// CHECK-NEXT: node_19 -> node_20 [label="blocking host wait
// CHECK-SAME: ltail=cluster_19 lhead=cluster_20];
// CHECK-NEXT: cluster_21 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_21[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="darkgreen"];
// CHECK-NEXT: }
// CHECK-NEXT: node_20 -> node_21 [label="blocking host wait
// CHECK-SAME: ltail=cluster_20 lhead=cluster_21];
// CHECK-NEXT: cluster_22 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_22[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="darkkhaki"];
// CHECK-NEXT: }
// CHECK-NEXT: node_21 -> node_22 [label="blocking host wait
// CHECK-SAME: ltail=cluster_21 lhead=cluster_22];
// CHECK-NEXT: cluster_23 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_23[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="darkmagenta"];
// CHECK-NEXT: }
// CHECK-NEXT: node_22 -> node_23 [label="blocking host wait
// CHECK-SAME: ltail=cluster_22 lhead=cluster_23];
// CHECK-NEXT: cluster_24 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_24[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="darkolivegreen"];
// CHECK-NEXT: }
// CHECK-NEXT: node_23 -> node_24 [label="blocking host wait
// CHECK-SAME: ltail=cluster_23 lhead=cluster_24];
// CHECK-NEXT: cluster_25 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_25[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="darkorange"];
// CHECK-NEXT: }
// CHECK-NEXT: node_24 -> node_25 [label="blocking host wait
// CHECK-SAME: ltail=cluster_24 lhead=cluster_25];
// CHECK-NEXT: cluster_26 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_26[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="darkorchid"];
// CHECK-NEXT: }
// CHECK-NEXT: node_25 -> node_26 [label="blocking host wait
// CHECK-SAME: ltail=cluster_25 lhead=cluster_26];
// CHECK-NEXT: cluster_27 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_27[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="darkred"];
// CHECK-NEXT: }
// CHECK-NEXT: node_26 -> node_27 [label="blocking host wait
// CHECK-SAME: ltail=cluster_26 lhead=cluster_27];
// CHECK-NEXT: cluster_28 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_28[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="darksalmon"];
// CHECK-NEXT: }
// CHECK-NEXT: node_27 -> node_28 [label="blocking host wait
// CHECK-SAME: ltail=cluster_27 lhead=cluster_28];
// CHECK-NEXT: cluster_29 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_29[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="darkseagreen"];
// CHECK-NEXT: }
// CHECK-NEXT: node_28 -> node_29 [label="blocking host wait
// CHECK-SAME: ltail=cluster_28 lhead=cluster_29];
// CHECK-NEXT: cluster_30 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_30[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="darkslateblue"];
// CHECK-NEXT: }
// CHECK-NEXT: node_29 -> node_30 [label="blocking host wait
// CHECK-SAME: ltail=cluster_29 lhead=cluster_30];
// CHECK-NEXT: cluster_31 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_31[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="darkslategray"];
// CHECK-NEXT: }
// CHECK-NEXT: node_30 -> node_31 [label="blocking host wait
// CHECK-SAME: ltail=cluster_30 lhead=cluster_31];
// CHECK-NEXT: cluster_32 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_32[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="darkturquoise"];
// CHECK-NEXT: }
// CHECK-NEXT: node_31 -> node_32 [label="blocking host wait
// CHECK-SAME: ltail=cluster_31 lhead=cluster_32];
// CHECK-NEXT: cluster_33 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_33[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="darkviolet"];
// CHECK-NEXT: }
// CHECK-NEXT: node_32 -> node_33 [label="blocking host wait
// CHECK-SAME: ltail=cluster_32 lhead=cluster_33];
// CHECK-NEXT: cluster_34 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_34[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="deeppink"];
// CHECK-NEXT: }
// CHECK-NEXT: node_33 -> node_34 [label="blocking host wait
// CHECK-SAME: ltail=cluster_33 lhead=cluster_34];
// CHECK-NEXT: cluster_35 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_35[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="deepskyblue"];
// CHECK-NEXT: }
// CHECK-NEXT: node_34 -> node_35 [label="blocking host wait
// CHECK-SAME: ltail=cluster_34 lhead=cluster_35];
// CHECK-NEXT: cluster_36 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_36[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="dimgray"];
// CHECK-NEXT: }
// CHECK-NEXT: node_35 -> node_36 [label="blocking host wait
// CHECK-SAME: ltail=cluster_35 lhead=cluster_36];
// CHECK-NEXT: cluster_37 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_37[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="dodgerblue"];
// CHECK-NEXT: }
// CHECK-NEXT: node_36 -> node_37 [label="blocking host wait
// CHECK-SAME: ltail=cluster_36 lhead=cluster_37];
// CHECK-NEXT: cluster_38 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_38[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="firebrick"];
// CHECK-NEXT: }
// CHECK-NEXT: node_37 -> node_38 [label="blocking host wait
// CHECK-SAME: ltail=cluster_37 lhead=cluster_38];
// CHECK-NEXT: cluster_39 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_39[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="floralwhite"];
// CHECK-NEXT: }
// CHECK-NEXT: node_38 -> node_39 [label="blocking host wait
// CHECK-SAME: ltail=cluster_38 lhead=cluster_39];
// CHECK-NEXT: cluster_40 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_40[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="forestgreen"];
// CHECK-NEXT: }
// CHECK-NEXT: node_39 -> node_40 [label="blocking host wait
// CHECK-SAME: ltail=cluster_39 lhead=cluster_40];
// CHECK-NEXT: cluster_41 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_41[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="fuchsia"];
// CHECK-NEXT: }
// CHECK-NEXT: node_40 -> node_41 [label="blocking host wait
// CHECK-SAME: ltail=cluster_40 lhead=cluster_41];
// CHECK-NEXT: cluster_42 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_42[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="gainsboro"];
// CHECK-NEXT: }
// CHECK-NEXT: node_41 -> node_42 [label="blocking host wait
// CHECK-SAME: ltail=cluster_41 lhead=cluster_42];
// CHECK-NEXT: cluster_43 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_43[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="gold"];
// CHECK-NEXT: }
// CHECK-NEXT: node_42 -> node_43 [label="blocking host wait
// CHECK-SAME: ltail=cluster_42 lhead=cluster_43];
// CHECK-NEXT: cluster_44 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_44[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="green"];
// CHECK-NEXT: }
// CHECK-NEXT: node_43 -> node_44 [label="blocking host wait
// CHECK-SAME: ltail=cluster_43 lhead=cluster_44];
// CHECK-NEXT: cluster_45 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_45[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="greenyellow"];
// CHECK-NEXT: }
// CHECK-NEXT: node_44 -> node_45 [label="blocking host wait
// CHECK-SAME: ltail=cluster_44 lhead=cluster_45];
// CHECK-NEXT: cluster_46 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_46[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="greenyellow"];
// CHECK-NEXT: }
// CHECK-NEXT: node_45 -> node_46 [label="blocking host wait
// CHECK-SAME: ltail=cluster_45 lhead=cluster_46];
// CHECK-NEXT: cluster_47 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_47[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="indianred"];
// CHECK-NEXT: }
// CHECK-NEXT: node_46 -> node_47 [label="blocking host wait
// CHECK-SAME: ltail=cluster_46 lhead=cluster_47];
// CHECK-NEXT: cluster_48 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_48[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="indigo"];
// CHECK-NEXT: }
// CHECK-NEXT: node_47 -> node_48 [label="blocking host wait
// CHECK-SAME: ltail=cluster_47 lhead=cluster_48];
// CHECK-NEXT: cluster_49 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_49[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="khaki"];
// CHECK-NEXT: }
// CHECK-NEXT: node_48 -> node_49 [label="blocking host wait
// CHECK-SAME: ltail=cluster_48 lhead=cluster_49];
// CHECK-NEXT: cluster_50 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_50[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="lavender"];
// CHECK-NEXT: }
// CHECK-NEXT: node_49 -> node_50 [label="blocking host wait
// CHECK-SAME: ltail=cluster_49 lhead=cluster_50];
// CHECK-NEXT: cluster_51 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_51[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="lawngreen"];
// CHECK-NEXT: }
// CHECK-NEXT: node_50 -> node_51 [label="blocking host wait
// CHECK-SAME: ltail=cluster_50 lhead=cluster_51];
// CHECK-NEXT: cluster_52 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_52[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="lemonchiffon"];
// CHECK-NEXT: }
// CHECK-NEXT: node_51 -> node_52 [label="blocking host wait
// CHECK-SAME: ltail=cluster_51 lhead=cluster_52];
// CHECK-NEXT: cluster_53 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_53[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="lightblue"];
// CHECK-NEXT: }
// CHECK-NEXT: node_52 -> node_53 [label="blocking host wait
// CHECK-SAME: ltail=cluster_52 lhead=cluster_53];
// CHECK-NEXT: cluster_54 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_54[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="lightcoral"];
// CHECK-NEXT: }
// CHECK-NEXT: node_53 -> node_54 [label="blocking host wait
// CHECK-SAME: ltail=cluster_53 lhead=cluster_54];
// CHECK-NEXT: cluster_55 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_55[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="lightgoldenrod"];
// CHECK-NEXT: }
// CHECK-NEXT: node_54 -> node_55 [label="blocking host wait
// CHECK-SAME: ltail=cluster_54 lhead=cluster_55];
// CHECK-NEXT: cluster_56 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_56[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="lightgreen"];
// CHECK-NEXT: }
// CHECK-NEXT: node_55 -> node_56 [label="blocking host wait
// CHECK-SAME: ltail=cluster_55 lhead=cluster_56];
// CHECK-NEXT: cluster_57 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_57[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="lightpink"];
// CHECK-NEXT: }
// CHECK-NEXT: node_56 -> node_57 [label="blocking host wait
// CHECK-SAME: ltail=cluster_56 lhead=cluster_57];
// CHECK-NEXT: cluster_58 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_58[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="lightsalmon"];
// CHECK-NEXT: }
// CHECK-NEXT: node_57 -> node_58 [label="blocking host wait
// CHECK-SAME: ltail=cluster_57 lhead=cluster_58];
// CHECK-NEXT: cluster_59 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_59[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="lightseagreen"];
// CHECK-NEXT: }
// CHECK-NEXT: node_58 -> node_59 [label="blocking host wait
// CHECK-SAME: ltail=cluster_58 lhead=cluster_59];
// CHECK-NEXT: cluster_60 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_60[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="lightsteelblue"];
// CHECK-NEXT: }
// CHECK-NEXT: node_59 -> node_60 [label="blocking host wait
// CHECK-SAME: ltail=cluster_59 lhead=cluster_60];
// CHECK-NEXT: cluster_61 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_61[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="lime"];
// CHECK-NEXT: }
// CHECK-NEXT: node_60 -> node_61 [label="blocking host wait
// CHECK-SAME: ltail=cluster_60 lhead=cluster_61];
// CHECK-NEXT: cluster_62 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_62[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="limegreen"];
// CHECK-NEXT: }
// CHECK-NEXT: node_61 -> node_62 [label="blocking host wait
// CHECK-SAME: ltail=cluster_61 lhead=cluster_62];
// CHECK-NEXT: cluster_63 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_63[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="magenta"];
// CHECK-NEXT: }
// CHECK-NEXT: node_62 -> node_63 [label="blocking host wait
// CHECK-SAME: ltail=cluster_62 lhead=cluster_63];
// CHECK-NEXT: cluster_64 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_64[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="maroon"];
// CHECK-NEXT: }
// CHECK-NEXT: node_63 -> node_64 [label="blocking host wait
// CHECK-SAME: ltail=cluster_63 lhead=cluster_64];
// CHECK-NEXT: cluster_65 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_65[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="mediumaquamarine"];
// CHECK-NEXT: }
// CHECK-NEXT: node_64 -> node_65 [label="blocking host wait
// CHECK-SAME: ltail=cluster_64 lhead=cluster_65];
// CHECK-NEXT: cluster_66 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_66[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="mediumorchid"];
// CHECK-NEXT: }
// CHECK-NEXT: node_65 -> node_66 [label="blocking host wait
// CHECK-SAME: ltail=cluster_65 lhead=cluster_66];
// CHECK-NEXT: cluster_67 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_67[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="mediumpurple"];
// CHECK-NEXT: }
// CHECK-NEXT: node_66 -> node_67 [label="blocking host wait
// CHECK-SAME: ltail=cluster_66 lhead=cluster_67];
// CHECK-NEXT: cluster_68 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_68[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="mediumseagreen"];
// CHECK-NEXT: }
// CHECK-NEXT: node_67 -> node_68 [label="blocking host wait
// CHECK-SAME: ltail=cluster_67 lhead=cluster_68];
// CHECK-NEXT: cluster_69 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_69[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="mediumvioletred"];
// CHECK-NEXT: }
// CHECK-NEXT: node_68 -> node_69 [label="blocking host wait
// CHECK-SAME: ltail=cluster_68 lhead=cluster_69];
// CHECK-NEXT: cluster_70 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_70[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="moccasin"];
// CHECK-NEXT: }
// CHECK-NEXT: node_69 -> node_70 [label="blocking host wait
// CHECK-SAME: ltail=cluster_69 lhead=cluster_70];
// CHECK-NEXT: cluster_71 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_71[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="olive"];
// CHECK-NEXT: }
// CHECK-NEXT: node_70 -> node_71 [label="blocking host wait
// CHECK-SAME: ltail=cluster_70 lhead=cluster_71];
// CHECK-NEXT: cluster_72 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_72[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="orange"];
// CHECK-NEXT: }
// CHECK-NEXT: node_71 -> node_72 [label="blocking host wait
// CHECK-SAME: ltail=cluster_71 lhead=cluster_72];
// CHECK-NEXT: cluster_73 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_73[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="orangered"];
// CHECK-NEXT: }
// CHECK-NEXT: node_72 -> node_73 [label="blocking host wait
// CHECK-SAME: ltail=cluster_72 lhead=cluster_73];
// CHECK-NEXT: cluster_74 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_74[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="orchid"];
// CHECK-NEXT: }
// CHECK-NEXT: node_73 -> node_74 [label="blocking host wait
// CHECK-SAME: ltail=cluster_73 lhead=cluster_74];
// CHECK-NEXT: cluster_75 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_75[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="palegreen"];
// CHECK-NEXT: }
// CHECK-NEXT: node_74 -> node_75 [label="blocking host wait
// CHECK-SAME: ltail=cluster_74 lhead=cluster_75];
// CHECK-NEXT: cluster_76 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_76[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="paleturquoise"];
// CHECK-NEXT: }
// CHECK-NEXT: node_75 -> node_76 [label="blocking host wait
// CHECK-SAME: ltail=cluster_75 lhead=cluster_76];
// CHECK-NEXT: cluster_77 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_77[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="palevioletred"];
// CHECK-NEXT: }
// CHECK-NEXT: node_76 -> node_77 [label="blocking host wait
// CHECK-SAME: ltail=cluster_76 lhead=cluster_77];
// CHECK-NEXT: cluster_78 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_78[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="peachpuff"];
// CHECK-NEXT: }
// CHECK-NEXT: node_77 -> node_78 [label="blocking host wait
// CHECK-SAME: ltail=cluster_77 lhead=cluster_78];
// CHECK-NEXT: cluster_79 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_79[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="peru"];
// CHECK-NEXT: }
// CHECK-NEXT: node_78 -> node_79 [label="blocking host wait
// CHECK-SAME: ltail=cluster_78 lhead=cluster_79];
// CHECK-NEXT: cluster_80 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_80[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="pink"];
// CHECK-NEXT: }
// CHECK-NEXT: node_79 -> node_80 [label="blocking host wait
// CHECK-SAME: ltail=cluster_79 lhead=cluster_80];
// CHECK-NEXT: cluster_81 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_81[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="plum"];
// CHECK-NEXT: }
// CHECK-NEXT: node_80 -> node_81 [label="blocking host wait
// CHECK-SAME: ltail=cluster_80 lhead=cluster_81];
// CHECK-NEXT: cluster_82 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_82[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="powderblue"];
// CHECK-NEXT: }
// CHECK-NEXT: node_81 -> node_82 [label="blocking host wait
// CHECK-SAME: ltail=cluster_81 lhead=cluster_82];
// CHECK-NEXT: cluster_83 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_83[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="purple"];
// CHECK-NEXT: }
// CHECK-NEXT: node_82 -> node_83 [label="blocking host wait
// CHECK-SAME: ltail=cluster_82 lhead=cluster_83];
// CHECK-NEXT: cluster_84 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_84[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="red"];
// CHECK-NEXT: }
// CHECK-NEXT: node_83 -> node_84 [label="blocking host wait
// CHECK-SAME: ltail=cluster_83 lhead=cluster_84];
// CHECK-NEXT: cluster_85 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_85[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="rosybrown"];
// CHECK-NEXT: }
// CHECK-NEXT: node_84 -> node_85 [label="blocking host wait
// CHECK-SAME: ltail=cluster_84 lhead=cluster_85];
// CHECK-NEXT: cluster_86 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_86[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="salmon"];
// CHECK-NEXT: }
// CHECK-NEXT: node_85 -> node_86 [label="blocking host wait
// CHECK-SAME: ltail=cluster_85 lhead=cluster_86];
// CHECK-NEXT: cluster_87 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_87[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="sandybrown"];
// CHECK-NEXT: }
// CHECK-NEXT: node_86 -> node_87 [label="blocking host wait
// CHECK-SAME: ltail=cluster_86 lhead=cluster_87];
// CHECK-NEXT: cluster_88 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_88[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="seagreen"];
// CHECK-NEXT: }
// CHECK-NEXT: node_87 -> node_88 [label="blocking host wait
// CHECK-SAME: ltail=cluster_87 lhead=cluster_88];
// CHECK-NEXT: cluster_89 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_89[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="sienna"];
// CHECK-NEXT: }
// CHECK-NEXT: node_88 -> node_89 [label="blocking host wait
// CHECK-SAME: ltail=cluster_88 lhead=cluster_89];
// CHECK-NEXT: cluster_90 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_90[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="silver"];
// CHECK-NEXT: }
// CHECK-NEXT: node_89 -> node_90 [label="blocking host wait
// CHECK-SAME: ltail=cluster_89 lhead=cluster_90];
// CHECK-NEXT: cluster_91 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_91[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="skyblue"];
// CHECK-NEXT: }
// CHECK-NEXT: node_90 -> node_91 [label="blocking host wait
// CHECK-SAME: ltail=cluster_90 lhead=cluster_91];
// CHECK-NEXT: cluster_92 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_92[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="slateblue"];
// CHECK-NEXT: }
// CHECK-NEXT: node_91 -> node_92 [label="blocking host wait
// CHECK-SAME: ltail=cluster_91 lhead=cluster_92];
// CHECK-NEXT: cluster_93 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_93[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="slategrey"];
// CHECK-NEXT: }
// CHECK-NEXT: node_92 -> node_93 [label="blocking host wait
// CHECK-SAME: ltail=cluster_92 lhead=cluster_93];
// CHECK-NEXT: cluster_94 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_94[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="springgreen"];
// CHECK-NEXT: }
// CHECK-NEXT: node_93 -> node_94 [label="blocking host wait
// CHECK-SAME: ltail=cluster_93 lhead=cluster_94];
// CHECK-NEXT: cluster_95 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_95[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="steelblue"];
// CHECK-NEXT: }
// CHECK-NEXT: node_94 -> node_95 [label="blocking host wait
// CHECK-SAME: ltail=cluster_94 lhead=cluster_95];
// CHECK-NEXT: cluster_96 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_96[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="tan"];
// CHECK-NEXT: }
// CHECK-NEXT: node_95 -> node_96 [label="blocking host wait
// CHECK-SAME: ltail=cluster_95 lhead=cluster_96];
// CHECK-NEXT: cluster_97 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_97[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="teal"];
// CHECK-NEXT: }
// CHECK-NEXT: node_96 -> node_97 [label="blocking host wait
// CHECK-SAME: ltail=cluster_96 lhead=cluster_97];
// CHECK-NEXT: cluster_98 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_98[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="thistle"];
// CHECK-NEXT: }
// CHECK-NEXT: node_97 -> node_98 [label="blocking host wait
// CHECK-SAME: ltail=cluster_97 lhead=cluster_98];
// CHECK-NEXT: cluster_99 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_99[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="tomato"];
// CHECK-NEXT: }
// CHECK-NEXT: node_98 -> node_99 [label="blocking host wait
// CHECK-SAME: ltail=cluster_98 lhead=cluster_99];
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "common.h"
#include <array>

int main() {
  CLState State;

  constexpr unsigned N = 100;
  std::array<cl_command_queue, N> Queues;
  for (unsigned i = 0; i < N; i++) {
    cl_int Ret = CL_SUCCESS;
    Queues[i] = clCreateCommandQueue(State.Context, State.Device, 0, &Ret);
    CHECK(Ret);

    Ret =
        clEnqueueNDRangeKernel(Queues[i], State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 0, nullptr, nullptr);

    Ret = clFinish(Queues[i]);
    CHECK(Ret);
  }

  for (unsigned i = 0; i < N; i++) {
    if (cl_command_queue Queue = Queues[i]; Queue != nullptr) {
      clReleaseCommandQueue(Queue);
    }
  }

  return 0;
}
